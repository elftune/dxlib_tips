/*
	2019/01/27	タイピングソフトサンプル by elftune
	https://dxlib.xsrv.jp/cgi/patiobbs/patio.cgi?mode=view&no=4567 にDxLib管理人さんが書いたコードを発展
*/
#include "DxLib.h"

// 文字変換データ
struct CONVDATA
{
	// ローマ仮名に変換する前の文字列
	TCHAR *BaseString;

	// 入力するアルファベットの文字列
	TCHAR *InputChars[32]; // 10じゃ足りんかった...
};


// https://support.microsoft.com/ja-jp/help/883232 を参照
CONVDATA StringConvTable[] =
{
	{ TEXT("うぁ"), { TEXT("wha"),TEXT("ula"),TEXT("uxa"),TEXT("wula"),TEXT("wuxa"),TEXT("whula"),TEXT("whuxa")} },
	{ TEXT("うぃ"), { TEXT("whi"),TEXT("wi"),TEXT("uli"),TEXT("uxi"),TEXT("ulyi"),TEXT("uxyi"),TEXT("wuli"),TEXT("wuxi"),TEXT("wulyi"),TEXT("wuxyi"),TEXT("whuli"),TEXT("whuxi"),TEXT("whulyi"),TEXT("whuxyi")} },
	{ TEXT("うぇ"), { TEXT("whe"),TEXT("we"),TEXT("ule"),TEXT("uxe"),TEXT("ulye"),TEXT("uxye"),TEXT("wule"),TEXT("wuxe"),TEXT("wulye"),TEXT("wuxye"),TEXT("whule"),TEXT("whuxe"),TEXT("whulye"),TEXT("whuxye")} },
	{ TEXT("うぉ"), { TEXT("who"),TEXT("ulo"),TEXT("uxo"),TEXT("wulo"),TEXT("wuxo"),TEXT("whulo"),TEXT("whuxo")} },
	{ TEXT("ぃぇ"), { TEXT("ye"),TEXT("lile"),TEXT("lixe"),TEXT("lilye"),TEXT("lixye"),TEXT("xile"),TEXT("xixe"),TEXT("xilye"),TEXT("xixye"),TEXT("lyile"),TEXT("lyixe"),TEXT("lyilye"),TEXT("lyixye"),TEXT("xyile"),TEXT("xyixe"),TEXT("xyilye"),TEXT("xyixye")} },
	{ TEXT("きゃ"), { TEXT("kya"),TEXT("kilya"),TEXT("kixya")} },
	{ TEXT("きぃ"), { TEXT("kyi"),TEXT("kili"),TEXT("kixi"),TEXT("kilyi"),TEXT("kixyi")} },
	{ TEXT("きゅ"), { TEXT("kyu"),TEXT("kilyu"),TEXT("kixyu")} },
	{ TEXT("きぇ"), { TEXT("kye"),TEXT("kile"),TEXT("kixe"),TEXT("kilye"),TEXT("kixye")} },
	{ TEXT("きょ"), { TEXT("kyo"),TEXT("kilyo"),TEXT("kixyo")} },
	{ TEXT("くゃ"), { TEXT("qya"),TEXT("kulya"),TEXT("kuxya"),TEXT("culya"),TEXT("cuxya"),TEXT("qulya"),TEXT("quxya")} },
	{ TEXT("くゅ"), { TEXT("qyu"),TEXT("kulyu"),TEXT("kuxyu"),TEXT("culyu"),TEXT("cuxyu"),TEXT("qulyu"),TEXT("quxyu")} },
	{ TEXT("くょ"), { TEXT("qyo"),TEXT("kulyo"),TEXT("kuxyo"),TEXT("culyo"),TEXT("cuxyo"),TEXT("qulyo"),TEXT("quxyo")} },
	{ TEXT("くぁ"), { TEXT("qwa"),TEXT("qa"),TEXT("kwa"),TEXT("kula"),TEXT("kuxa"),TEXT("cula"),TEXT("cuxa"),TEXT("qula"),TEXT("quxa")} },
	{ TEXT("くぃ"), { TEXT("qwi"),TEXT("qi"),TEXT("qyi"),TEXT("kuli"),TEXT("kuxi"),TEXT("kulyi"),TEXT("kuxyi"),TEXT("culi"),TEXT("cuxi"),TEXT("culyi"),TEXT("cuxyi"),TEXT("quli"),TEXT("quxi"),TEXT("qulyi"),TEXT("quxyi")} },
	{ TEXT("くぅ"), { TEXT("qwu"),TEXT("kulu"),TEXT("kuxu"),TEXT("culu"),TEXT("cuxu"),TEXT("qulu"),TEXT("quxu")} },
	{ TEXT("くぇ"), { TEXT("qwe"),TEXT("qe"),TEXT("qye"),TEXT("kule"),TEXT("kuxe"),TEXT("kulye"),TEXT("kuxye"),TEXT("cule"),TEXT("cuxe"),TEXT("culye"),TEXT("cuxye"),TEXT("qule"),TEXT("quxe"),TEXT("qulye"),TEXT("quxye")} },
	{ TEXT("くぉ"), { TEXT("qwo"),TEXT("qo"),TEXT("kulo"),TEXT("kuxo"),TEXT("culo"),TEXT("cuxo"),TEXT("qulo"),TEXT("quxo")} },
	{ TEXT("ぎゃ"), { TEXT("gya"),TEXT("gilya"),TEXT("gixya")} },
	{ TEXT("ぎぃ"), { TEXT("gyi"),TEXT("gili"),TEXT("gixi"),TEXT("gilyi"),TEXT("gixyi")} },
	{ TEXT("ぎゅ"), { TEXT("gyu"),TEXT("gilyu"),TEXT("gixyu")} },
	{ TEXT("ぎぇ"), { TEXT("gye"),TEXT("gile"),TEXT("gixe"),TEXT("gilye"),TEXT("gixye")} },
	{ TEXT("ぎょ"), { TEXT("gyo"),TEXT("gilyo"),TEXT("gixyo")} },
	{ TEXT("ぐぁ"), { TEXT("gwa"),TEXT("gula"),TEXT("guxa")} },
	{ TEXT("ぐぃ"), { TEXT("gwi"),TEXT("guli"),TEXT("guxi"),TEXT("gulyi"),TEXT("guxyi")} },
	{ TEXT("ぐぅ"), { TEXT("gwu"),TEXT("gulu"),TEXT("guxu")} },
	{ TEXT("ぐぇ"), { TEXT("gwe"),TEXT("gule"),TEXT("guxe"),TEXT("gulye"),TEXT("guxye")} },
	{ TEXT("ぐぉ"), { TEXT("gwo"),TEXT("gulo"),TEXT("guxo")} },
	{ TEXT("しゃ"), { TEXT("sya"),TEXT("sha"),TEXT("silya"),TEXT("sixya"),TEXT("cilya"),TEXT("cixya"),TEXT("shilya"),TEXT("shixya")} },
	{ TEXT("しぃ"), { TEXT("syi"),TEXT("sili"),TEXT("sixi"),TEXT("silyi"),TEXT("sixyi"),TEXT("cili"),TEXT("cixi"),TEXT("cilyi"),TEXT("cixyi"),TEXT("shili"),TEXT("shixi"),TEXT("shilyi"),TEXT("shixyi")} },
	{ TEXT("しゅ"), { TEXT("syu"),TEXT("shu"),TEXT("silyu"),TEXT("sixyu"),TEXT("cilyu"),TEXT("cixyu"),TEXT("shilyu"),TEXT("shixyu")} },
	{ TEXT("しぇ"), { TEXT("sye"),TEXT("she"),TEXT("sile"),TEXT("sixe"),TEXT("silye"),TEXT("sixye"),TEXT("cile"),TEXT("cixe"),TEXT("cilye"),TEXT("cixye"),TEXT("shile"),TEXT("shixe"),TEXT("shilye"),TEXT("shixye")} },
	{ TEXT("しょ"), { TEXT("syo"),TEXT("sho"),TEXT("silyo"),TEXT("sixyo"),TEXT("cilyo"),TEXT("cixyo"),TEXT("shilyo"),TEXT("shixyo")} },
	{ TEXT("すぁ"), { TEXT("swa"),TEXT("sula"),TEXT("suxa")} },
	{ TEXT("すぃ"), { TEXT("swi"),TEXT("suli"),TEXT("suxi"),TEXT("sulyi"),TEXT("suxyi")} },
	{ TEXT("すぅ"), { TEXT("swu"),TEXT("sulu"),TEXT("suxu")} },
	{ TEXT("すぇ"), { TEXT("swe"),TEXT("sule"),TEXT("suxe"),TEXT("sulye"),TEXT("suxye")} },
	{ TEXT("すぉ"), { TEXT("swo"),TEXT("sulo"),TEXT("suxo")} },
	{ TEXT("じゃ"), { TEXT("zya"),TEXT("ja"),TEXT("jya"),TEXT("zilya"),TEXT("zixya"),TEXT("jilya"),TEXT("jixya")} },
	{ TEXT("じぃ"), { TEXT("zyi"),TEXT("jyi"),TEXT("zili"),TEXT("zixi"),TEXT("zilyi"),TEXT("zixyi"),TEXT("jili"),TEXT("jixi"),TEXT("jilyi"),TEXT("jixyi")} },
	{ TEXT("じゅ"), { TEXT("zyu"),TEXT("ju"),TEXT("jyu"),TEXT("zilyu"),TEXT("zixyu"),TEXT("jilyu"),TEXT("jixyu")} },
	{ TEXT("じぇ"), { TEXT("zye"),TEXT("je"),TEXT("jye"),TEXT("zile"),TEXT("zixe"),TEXT("zilye"),TEXT("zixye"),TEXT("jile"),TEXT("jixe"),TEXT("jilye"),TEXT("jixye")} },
	{ TEXT("じょ"), { TEXT("zyo"),TEXT("jo"),TEXT("jyo"),TEXT("zilyo"),TEXT("zixyo"),TEXT("jilyo"),TEXT("jixyo")} },
	{ TEXT("ちゃ"), { TEXT("tya"),TEXT("cha"),TEXT("cya"),TEXT("tilya"),TEXT("tixya"),TEXT("chilya"),TEXT("chixya")} },
	{ TEXT("ちぃ"), { TEXT("tyi"),TEXT("cyi"),TEXT("tili"),TEXT("tixi"),TEXT("tilyi"),TEXT("tixyi"),TEXT("chili"),TEXT("chixi"),TEXT("chilyi"),TEXT("chixyi")} },
	{ TEXT("ちゅ"), { TEXT("tyu"),TEXT("chu"),TEXT("cyu"),TEXT("tilyu"),TEXT("tixyu"),TEXT("chilyu"),TEXT("chixyu")} },
	{ TEXT("ちぇ"), { TEXT("tye"),TEXT("che"),TEXT("cye"),TEXT("tile"),TEXT("tixe"),TEXT("tilye"),TEXT("tixye"),TEXT("chile"),TEXT("chixe"),TEXT("chilye"),TEXT("chixye")} },
	{ TEXT("ちょ"), { TEXT("tyo"),TEXT("cho"),TEXT("cyo"),TEXT("tilyo"),TEXT("tixyo"),TEXT("chilyo"),TEXT("chixyo")} },
	{ TEXT("つぁ"), { TEXT("tsa"),TEXT("tula"),TEXT("tuxa"),TEXT("tsula"),TEXT("tsuxa")} },
	{ TEXT("つぃ"), { TEXT("tsi"),TEXT("tuli"),TEXT("tuxi"),TEXT("tulyi"),TEXT("tuxyi"),TEXT("tsuli"),TEXT("tsuxi"),TEXT("tsulyi"),TEXT("tsuxyi")} },
	{ TEXT("つぇ"), { TEXT("tse"),TEXT("tule"),TEXT("tuxe"),TEXT("tulye"),TEXT("tuxye"),TEXT("tsule"),TEXT("tsuxe"),TEXT("tsulye"),TEXT("tsuxye")} },
	{ TEXT("つぉ"), { TEXT("tso"),TEXT("tulo"),TEXT("tuxo"),TEXT("tsulo"),TEXT("tsuxo")} },
	{ TEXT("てゃ"), { TEXT("tha"),TEXT("telya"),TEXT("texya")} },
	{ TEXT("てぃ"), { TEXT("thi"),TEXT("teli"),TEXT("texi"),TEXT("telyi"),TEXT("texyi")} },
	{ TEXT("てゅ"), { TEXT("thu"),TEXT("telyu"),TEXT("texyu")} },
	{ TEXT("てぇ"), { TEXT("the"),TEXT("tele"),TEXT("texe"),TEXT("telye"),TEXT("texye")} },
	{ TEXT("てょ"), { TEXT("tho"),TEXT("telyo"),TEXT("texyo")} },
	{ TEXT("とぁ"), { TEXT("twa"),TEXT("tola"),TEXT("toxa")} },
	{ TEXT("とぃ"), { TEXT("twi"),TEXT("toli"),TEXT("toxi"),TEXT("tolyi"),TEXT("toxyi")} },
	{ TEXT("とぅ"), { TEXT("twu"),TEXT("tolu"),TEXT("toxu")} },
	{ TEXT("とぇ"), { TEXT("twe"),TEXT("tole"),TEXT("toxe"),TEXT("tolye"),TEXT("toxye")} },
	{ TEXT("とぉ"), { TEXT("two"),TEXT("tolo"),TEXT("toxo")} },
	{ TEXT("ぢゃ"), { TEXT("dya"),TEXT("dilya"),TEXT("dixya")} },
	{ TEXT("ぢぃ"), { TEXT("dyi"),TEXT("dili"),TEXT("dixi"),TEXT("dilyi"),TEXT("dixyi")} },
	{ TEXT("ぢゅ"), { TEXT("dyu"),TEXT("dilyu"),TEXT("dixyu")} },
	{ TEXT("ぢぇ"), { TEXT("dye"),TEXT("dile"),TEXT("dixe"),TEXT("dilye"),TEXT("dixye")} },
	{ TEXT("ぢょ"), { TEXT("dyo"),TEXT("dilyo"),TEXT("dixyo")} },
	{ TEXT("でゃ"), { TEXT("dha"),TEXT("delya"),TEXT("dexya")} },
	{ TEXT("でぃ"), { TEXT("dhi"),TEXT("deli"),TEXT("dexi"),TEXT("delyi"),TEXT("dexyi")} },
	{ TEXT("でゅ"), { TEXT("dhu"),TEXT("delyu"),TEXT("dexyu")} },
	{ TEXT("でぇ"), { TEXT("dhe"),TEXT("dele"),TEXT("dexe"),TEXT("delye"),TEXT("dexye")} },
	{ TEXT("でょ"), { TEXT("dho"),TEXT("delyo"),TEXT("dexyo")} },
	{ TEXT("どぁ"), { TEXT("dwa"),TEXT("dola"),TEXT("doxa")} },
	{ TEXT("どぃ"), { TEXT("dwi"),TEXT("doli"),TEXT("doxi"),TEXT("dolyi"),TEXT("doxyi")} },
	{ TEXT("どぅ"), { TEXT("dwu"),TEXT("dolu"),TEXT("doxu")} },
	{ TEXT("どぇ"), { TEXT("dwe"),TEXT("dole"),TEXT("doxe"),TEXT("dolye"),TEXT("doxye")} },
	{ TEXT("どぉ"), { TEXT("dwo"),TEXT("dolo"),TEXT("doxo")} },
	{ TEXT("にゃ"), { TEXT("nya"),TEXT("nilya"),TEXT("nixya")} },
	{ TEXT("にぃ"), { TEXT("nyi"),TEXT("nili"),TEXT("nixi"),TEXT("nilyi"),TEXT("nixyi")} },
	{ TEXT("にゅ"), { TEXT("nyu"),TEXT("nilyu"),TEXT("nixyu")} },
	{ TEXT("にぇ"), { TEXT("nye"),TEXT("nile"),TEXT("nixe"),TEXT("nilye"),TEXT("nixye")} },
	{ TEXT("にょ"), { TEXT("nyo"),TEXT("nilyo"),TEXT("nixyo")} },
	{ TEXT("ひゃ"), { TEXT("hya"),TEXT("hilya"),TEXT("hixya")} },
	{ TEXT("ひぃ"), { TEXT("hyi"),TEXT("hili"),TEXT("hixi"),TEXT("hilyi"),TEXT("hixyi")} },
	{ TEXT("ひゅ"), { TEXT("hyu"),TEXT("hilyu"),TEXT("hixyu")} },
	{ TEXT("ひぇ"), { TEXT("hye"),TEXT("hile"),TEXT("hixe"),TEXT("hilye"),TEXT("hixye")} },
	{ TEXT("ひょ"), { TEXT("hyo"),TEXT("hilyo"),TEXT("hixyo")} },
	{ TEXT("ふぁ"), { TEXT("fwa"),TEXT("fa"),TEXT("hula"),TEXT("huxa"),TEXT("fula"),TEXT("fuxa")} },
	{ TEXT("ふぃ"), { TEXT("fwi"),TEXT("fi"),TEXT("fyi"),TEXT("huli"),TEXT("huxi"),TEXT("hulyi"),TEXT("huxyi"),TEXT("fuli"),TEXT("fuxi"),TEXT("fulyi"),TEXT("fuxyi")} },
	{ TEXT("ふぅ"), { TEXT("fwu"),TEXT("hulu"),TEXT("huxu"),TEXT("fulu"),TEXT("fuxu")} },
	{ TEXT("ふぇ"), { TEXT("fwe"),TEXT("fe"),TEXT("fye"),TEXT("hule"),TEXT("huxe"),TEXT("hulye"),TEXT("huxye"),TEXT("fule"),TEXT("fuxe"),TEXT("fulye"),TEXT("fuxye")} },
	{ TEXT("ふぉ"), { TEXT("fwo"),TEXT("fo"),TEXT("hulo"),TEXT("huxo"),TEXT("fulo"),TEXT("fuxo")} },
	{ TEXT("ふゃ"), { TEXT("fya"),TEXT("hulya"),TEXT("huxya"),TEXT("fulya"),TEXT("fuxya")} },
	{ TEXT("ふゅ"), { TEXT("fyu"),TEXT("hulyu"),TEXT("huxyu"),TEXT("fulyu"),TEXT("fuxyu")} },
	{ TEXT("ふょ"), { TEXT("fyo"),TEXT("hulyo"),TEXT("huxyo"),TEXT("fulyo"),TEXT("fuxyo")} },
	{ TEXT("びゃ"), { TEXT("bya"),TEXT("bilya"),TEXT("bixya")} },
	{ TEXT("びぃ"), { TEXT("byi"),TEXT("bili"),TEXT("bixi"),TEXT("bilyi"),TEXT("bixyi")} },
	{ TEXT("びゅ"), { TEXT("byu"),TEXT("bilyu"),TEXT("bixyu")} },
	{ TEXT("びぇ"), { TEXT("bye"),TEXT("bile"),TEXT("bixe"),TEXT("bilye"),TEXT("bixye")} },
	{ TEXT("びょ"), { TEXT("byo"),TEXT("bilyo"),TEXT("bixyo")} },
	{ TEXT("ヴぁ"), { TEXT("va"),TEXT("vula"),TEXT("vuxa")} },
	{ TEXT("ヴぃ"), { TEXT("vi"),TEXT("vuli"),TEXT("vuxi"),TEXT("vulyi"),TEXT("vuxyi")} },
	{ TEXT("ヴぇ"), { TEXT("ve"),TEXT("vule"),TEXT("vuxe"),TEXT("vulye"),TEXT("vuxye")} },
	{ TEXT("ヴぉ"), { TEXT("vo"),TEXT("vulo"),TEXT("vuxo")} },
	{ TEXT("ヴゃ"), { TEXT("vya"),TEXT("vulya"),TEXT("vuxya")} },
	{ TEXT("ヴぃ"), { TEXT("vyi"),TEXT("vuli"),TEXT("vuxi"),TEXT("vulyi"),TEXT("vuxyi")} },
	{ TEXT("ヴゅ"), { TEXT("vyu"),TEXT("vulyu"),TEXT("vuxyu")} },
	{ TEXT("ヴぇ"), { TEXT("vye"),TEXT("vule"),TEXT("vuxe"),TEXT("vulye"),TEXT("vuxye")} },
	{ TEXT("ヴょ"), { TEXT("vyo"),TEXT("vulyo"),TEXT("vuxyo")} },
	{ TEXT("ぴゃ"), { TEXT("pya"),TEXT("pilya"),TEXT("pixya")} },
	{ TEXT("ぴぃ"), { TEXT("pyi"),TEXT("pili"),TEXT("pixi"),TEXT("pilyi"),TEXT("pixyi")} },
	{ TEXT("ぴゅ"), { TEXT("pyu"),TEXT("pilyu"),TEXT("pixyu")} },
	{ TEXT("ぴぇ"), { TEXT("pye"),TEXT("pile"),TEXT("pixe"),TEXT("pilye"),TEXT("pixye")} },
	{ TEXT("ぴょ"), { TEXT("pyo"),TEXT("pilyo"),TEXT("pixyo")} },
	{ TEXT("みゃ"), { TEXT("mya"),TEXT("milya"),TEXT("mixya")} },
	{ TEXT("みぃ"), { TEXT("myi"),TEXT("mili"),TEXT("mixi"),TEXT("milyi"),TEXT("mixyi")} },
	{ TEXT("みゅ"), { TEXT("myu"),TEXT("milyu"),TEXT("mixyu")} },
	{ TEXT("みぇ"), { TEXT("mye"),TEXT("mile"),TEXT("mixe"),TEXT("milye"),TEXT("mixye")} },
	{ TEXT("みょ"), { TEXT("myo"),TEXT("milyo"),TEXT("mixyo")} },
	{ TEXT("りゃ"), { TEXT("rya"),TEXT("rilya"),TEXT("rixya")} },
	{ TEXT("りぃ"), { TEXT("ryi"),TEXT("rili"),TEXT("rixi"),TEXT("rilyi"),TEXT("rixyi")} },
	{ TEXT("りゅ"), { TEXT("ryu"),TEXT("rilyu"),TEXT("rixyu")} },
	{ TEXT("りぇ"), { TEXT("rye"),TEXT("rile"),TEXT("rixe"),TEXT("rilye"),TEXT("rixye")} },
	{ TEXT("りょ"), { TEXT("ryo"),TEXT("rilyo"),TEXT("rixyo")} },
	{ TEXT("あ"), { TEXT("a")} },
	{ TEXT("い"), { TEXT("i"),TEXT("yi")} },
	{ TEXT("う"), { TEXT("u"),TEXT("wu"),TEXT("whu")} },
	{ TEXT("え"), { TEXT("e")} },
	{ TEXT("お"), { TEXT("o")} },
	{ TEXT("か"), { TEXT("ka"),TEXT("ca")} },
	{ TEXT("き"), { TEXT("ki")} },
	{ TEXT("く"), { TEXT("ku"),TEXT("cu"),TEXT("qu")} },
	{ TEXT("け"), { TEXT("ke")} },
	{ TEXT("こ"), { TEXT("ko"),TEXT("co")} },
	{ TEXT("さ"), { TEXT("sa")} },
	{ TEXT("し"), { TEXT("si"),TEXT("ci"),TEXT("shi")} },
	{ TEXT("す"), { TEXT("su")} },
	{ TEXT("せ"), { TEXT("se"),TEXT("ce")} },
	{ TEXT("そ"), { TEXT("so")} },
	{ TEXT("た"), { TEXT("ta")} },
	{ TEXT("ち"), { TEXT("ti"),TEXT("chi")} },
	{ TEXT("つ"), { TEXT("tu"),TEXT("tsu")} },
	{ TEXT("て"), { TEXT("te")} },
	{ TEXT("と"), { TEXT("to")} },
	{ TEXT("な"), { TEXT("na")} },
	{ TEXT("に"), { TEXT("ni")} },
	{ TEXT("ぬ"), { TEXT("nu")} },
	{ TEXT("ね"), { TEXT("ne")} },
	{ TEXT("の"), { TEXT("no")} },
	{ TEXT("は"), { TEXT("ha")} },
	{ TEXT("ひ"), { TEXT("hi")} },
	{ TEXT("ふ"), { TEXT("hu"),TEXT("fu")} },
	{ TEXT("へ"), { TEXT("he")} },
	{ TEXT("ほ"), { TEXT("ho")} },
	{ TEXT("ま"), { TEXT("ma")} },
	{ TEXT("み"), { TEXT("mi")} },
	{ TEXT("む"), { TEXT("mu")} },
	{ TEXT("め"), { TEXT("me")} },
	{ TEXT("も"), { TEXT("mo")} },
	{ TEXT("や"), { TEXT("ya")} },
	{ TEXT("ゆ"), { TEXT("yu")} },
	{ TEXT("よ"), { TEXT("yo")} },
	{ TEXT("ら"), { TEXT("ra")} },
	{ TEXT("り"), { TEXT("ri")} },
	{ TEXT("る"), { TEXT("ru")} },
	{ TEXT("れ"), { TEXT("re")} },
	{ TEXT("ろ"), { TEXT("ro")} },
	{ TEXT("わ"), { TEXT("wa")} },
	{ TEXT("を"), { TEXT("wo")} },
	{ TEXT("ん"), { TEXT("n"),TEXT("nn"),TEXT("n'"),TEXT("xn")} },
	{ TEXT("ゐ"), { TEXT("wi")} },
	{ TEXT("ゑ"), { TEXT("we")} },
	{ TEXT("ぁ"), { TEXT("la"),TEXT("xa")} },
	{ TEXT("ぃ"), { TEXT("li"),TEXT("xi"),TEXT("lyi"),TEXT("xyi")} },
	{ TEXT("ぅ"), { TEXT("lu"),TEXT("xu")} },
	{ TEXT("ぇ"), { TEXT("le"),TEXT("xe"),TEXT("lye"),TEXT("xye")} },
	{ TEXT("ぉ"), { TEXT("lo"),TEXT("xo")} },
	{ TEXT("が"), { TEXT("ga")} },
	{ TEXT("ぎ"), { TEXT("gi")} },
	{ TEXT("ぐ"), { TEXT("gu")} },
	{ TEXT("げ"), { TEXT("ge")} },
	{ TEXT("ご"), { TEXT("go")} },
	{ TEXT("ヵ"), { TEXT("lka"),TEXT("xka")} },
	{ TEXT("ヶ"), { TEXT("lke"),TEXT("xke")} },
	{ TEXT("ざ"), { TEXT("za")} },
	{ TEXT("じ"), { TEXT("zi"),TEXT("ji")} },
	{ TEXT("ず"), { TEXT("zu")} },
	{ TEXT("ぜ"), { TEXT("ze")} },
	{ TEXT("ぞ"), { TEXT("zo")} },
	{ TEXT("だ"), { TEXT("da")} },
	{ TEXT("ぢ"), { TEXT("di")} },
	{ TEXT("づ"), { TEXT("du")} },
	{ TEXT("で"), { TEXT("de")} },
	{ TEXT("ど"), { TEXT("do")} },
	{ TEXT("っ"), { TEXT("ltu"),TEXT("xtu"),TEXT("ltsu")} },
	{ TEXT("ば"), { TEXT("ba")} },
	{ TEXT("び"), { TEXT("bi")} },
	{ TEXT("ぶ"), { TEXT("bu")} },
	{ TEXT("べ"), { TEXT("be")} },
	{ TEXT("ぼ"), { TEXT("bo")} },
	{ TEXT("ヴ"), { TEXT("vu")} },
	{ TEXT("ぱ"), { TEXT("pa")} },
	{ TEXT("ぴ"), { TEXT("pi")} },
	{ TEXT("ぷ"), { TEXT("pu")} },
	{ TEXT("ぺ"), { TEXT("pe")} },
	{ TEXT("ぽ"), { TEXT("po")} },
	{ TEXT("ゃ"), { TEXT("lya"),TEXT("xya")} },
	{ TEXT("ゅ"), { TEXT("lyu"),TEXT("xyu")} },
	{ TEXT("ょ"), { TEXT("lyo"),TEXT("xyo")} },
	{ TEXT("ゎ"), { TEXT("lwa"),TEXT("xwa")} },
	{ TEXT("！"), { TEXT("!")} },
	{ TEXT("？"), { TEXT("?")} },
	{ TEXT("＠"), { TEXT("@")} },
	{ TEXT("ー"), { TEXT("-")} },
	{ TEXT("、"), { TEXT(",")} },
	{ TEXT("。"), { TEXT(".")} },
	{ TEXT("＝"), { TEXT("=")} },
	{ TEXT("（"), { TEXT("(")} },
	{ TEXT("）"), { TEXT(")")} },
	{ NULL, { NULL } }
};

struct TYPINGDATA
{
	// 入力すべき文字列を変換テーブルの対応するインデックスに置き換えたもの
	int TableIndex[512];

	// インデックスの数
	int TableIndexNum;

	// 入力が完了したインデックスの数
	int CompleteIndexNum;

	// 現在入力中の文字列
	TCHAR TypingString[64];

	// 入力が完了した文字列
	TCHAR CompletionString[512];
};

TYPINGDATA TypingData;

/*
StringConvTableを「2文字」群から記述しているのは、この関数で2文字群から先にヒット
させるため。（じゃ　が じ＋ゃ にならないようにするため）
*/
int ConvertString(TCHAR *pBaseString)
{
	::ZeroMemory(&TypingData, sizeof(TypingData));
	int nLen = (int)DxLib::strlenDx(pBaseString), k = 0;

	int i = 0;
	for (int j = 0; j < nLen; j++)
	{
		i = 0;
		while (StringConvTable[i].BaseString != NULL)
		{
			int n = (int)strlenDx(StringConvTable[i].BaseString);
#ifdef UNICODE
			switch (n)
			{
			case 1:
				if (*StringConvTable[i].BaseString == *(pBaseString + j))
				{
					TypingData.TableIndex[k++] = i;
					TypingData.TableIndexNum++;
					goto NEXT_LOOP;
				}
				break;
			case 2:
				if (*(pBaseString + j + 1) != '\0')
				{
					if (*(StringConvTable[i].BaseString + 0) == *(pBaseString + j) && *(StringConvTable[i].BaseString + 1) == *(pBaseString + j + 1))
					{
						TypingData.TableIndex[k++] = i;
						j++;
						TypingData.TableIndexNum++;
						goto NEXT_LOOP;
					}
				}
				break;
			default:
				break;
			}
#else
			switch (n)
			{
			case 2:
				if (*(StringConvTable[i].BaseString + 0) == *(pBaseString + j + 0) && *(StringConvTable[i].BaseString + 1) == *(pBaseString + j + 1))
				{
					TypingData.TableIndex[k++] = i;
					TypingData.TableIndexNum++;
					j += 1;
					goto NEXT_LOOP;
				}
				break;
			case 4:
				if (*(pBaseString + j + 4) != '\0')
				{
					if (*(StringConvTable[i].BaseString + 0) == *(pBaseString + j + 0) && *(StringConvTable[i].BaseString + 1) == *(pBaseString + j + 1) &&
						*(StringConvTable[i].BaseString + 2) == *(pBaseString + j + 2) && *(StringConvTable[i].BaseString + 3) == *(pBaseString + j + 3))
					{
						TypingData.TableIndex[k++] = i;
						j += 3;
						TypingData.TableIndexNum++;
						goto NEXT_LOOP;
					}
				}
				break;
			default:
				break;
			}
#endif
			i++;
		}
	NEXT_LOOP:;
	}

	return TypingData.TableIndexNum;
}

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
{
	DxLib::ChangeWindowMode(TRUE);
	DxLib::SetAlwaysRunFlag(TRUE);
	if (DxLib::DxLib_Init() < 0) return -1;
	DxLib::SetDrawScreen(DX_SCREEN_BACK);

	TCHAR *pStrQTxt = TEXT("そんなに結果重視ならじゃあ時間を測ろう"); // 全角文字のみにすること
	TCHAR *pStrKTxt = TEXT("そんなにけっかじゅうしならじゃあじかんをはかろう");

LOOP_INIT:
	ConvertString(pStrKTxt);
	int nSubCounter = 0;
	TCHAR cMiss = '\0';
	while (DxLib::ProcessMessage() == 0)
	{
		if (DxLib::CheckHitKey(KEY_INPUT_ESCAPE) == TRUE) goto LOOP_INIT;
		TCHAR c = DxLib::GetInputChar(TRUE);
		if (c != '\0' && c >= CTRL_CODE_CMP) {
			int i = 0;
			bool bOK = false, bDoing = false; // 入力完了（文字単位）と入力途中（文字単位）
			/*
			「ん」は n だけで完了する場合と、「ん＋な行」「ん＋母音」「ん＋ん」のように n だけではダメな場合がある

			「けっか」は「け ＋ っ ＋ か」か「け＋っか」
			*/
			CONVDATA *pData = &StringConvTable[TypingData.TableIndex[TypingData.CompleteIndexNum]]; // 文字indexの場所
			while (pData->InputChars[i] != NULL) // 1～数個あるので繰り返し
			{
				TCHAR c2 = *pData->InputChars[i];
				bool bN = false; // bNは「ん」のために対応が必要な時期を意味する　
				if (TypingData.CompleteIndexNum > 0) {
					if (DxLib::strcmpDx(StringConvTable[TypingData.TableIndex[TypingData.CompleteIndexNum - 1]].BaseString, TEXT("ん")) == 0 && (nSubCounter == 0)) // 前の文字が ん の場合
					{
						/* ん は n, nn, xn, n' なので、2文字入力されていれば問題ない。つまり2文字前が n か x なら何も考えなくてよい*/
						bool b2 = true; // んとして nが1文字、しか入力されていないか
						if ((int)DxLib::strlenDx(TypingData.CompletionString) >= 2) // 調べるにしても2文字は必要だ
						{
							TCHAR c3 = TypingData.CompletionString[(int)DxLib::strlenDx(TypingData.CompletionString) - 2];
							if (c3 == 'n' || c3 == 'x') b2 = false; // nやxが入力済ならもう済んでいる
						} // b2 = true なら n しか入力されていない状態

						if (b2 == true)
						{
							// 今入力すべき文字及び、入力した文字は nn にしなければならないものか
							bN = (c2 == 'a' || c2 == 'i' || c2 == 'u' || c2 == 'e' || c2 == 'o' || c2 == 'n' || c == 'n' || c == '\'');
							if (bN == true)
							{
								c2 = 'n'; // 入力すべき文字、を n にする。カウンターを1つ戻す(あとで+1するので0になる。最初から、にする)
								nSubCounter = -1;
							}
						}
					}

					if (DxLib::strcmpDx(pData->BaseString, TEXT("っ")) == 0) // 今の文字が っ の場合
					{
						// 次の文字があるか ... っ で終わった場合「えっ」とかは無視 (ltsuを入力させる）
						CONVDATA *pData2 = NULL;
						if (TypingData.TableIndexNum > TypingData.CompleteIndexNum)
						{
							pData2 = &StringConvTable[TypingData.TableIndex[TypingData.CompleteIndexNum + 1]]; // 次の文字indexの場所
							// 「っか」 kka のように っの次は子音(なんだが、ここではデータが正しいだろうからということで今回はチェックしていいない）
							int i2 = 0;
							while (pData2->InputChars[i2] != NULL)
							{
								if (*(pData2->InputChars[i2] + 0) == c) // 正しい文字を入力しているなら
								{
									c2 = *(pData2->InputChars[i2] + 0);
									nSubCounter--; // 1つ巻き戻す
									bOK = true; //「っ」を完了させる
									break;
								}
								i2++;
							}
						}
					}
				}

				if (nSubCounter >= 0)
				{
					if (bN == false) c2 = *(pData->InputChars[i] + nSubCounter);
				}

				if ((c2 == c) || (c == '\'' && c2 == 'n')) // 入力した文字が「どれかの途中段階」に一致か
				{
					bDoing = true; // 「いずれかの文字群を正しく入力中」フラグON
					nSubCounter++;
					if (bN == false)
					{
						if (nSubCounter == (int)DxLib::strlenDx(pData->InputChars[i])) //  「いずれかの文字群を正しく入力し終わった」なら完了
						{
							bOK = true;
						}
						break;
					}
					else {
						if (nSubCounter > (int)DxLib::strlenDx(pData->InputChars[i])) //  「いずれかの文字群を正しく入力し終わった」なら完了
						{
							bOK = true;
						}
						break;
					}
				}
				i++;
			}
			if (bOK == true) // 完了したならTypingData.CompletionStringに追加
			{
				TypingData.CompletionString[(int)DxLib::strlenDx(TypingData.CompletionString)] = c;
				cMiss = '\0'; // ミスった判定用の文字をクリア
				TypingData.CompleteIndexNum++;
				nSubCounter = 0; // 最初の文字から
			}
			else
			{
				if (bDoing == true) // 完了はしていないが、正しく入力中の場合
				{
					TypingData.CompletionString[(int)DxLib::strlenDx(TypingData.CompletionString)] = c;
					cMiss = '\0';
				}
				else // 間違った文字だった場合
				{
					cMiss = c;
				}
			}
		}

		DxLib::ClearDrawScreen();
		DxLib::DrawString(32, 32, pStrQTxt, DxLib::GetColor(255, 255, 255)); // 問題
		DxLib::DrawString(32, 64, pStrKTxt, DxLib::GetColor(255, 255, 0)); // カナ表記
		DxLib::DrawString(32, 96, TypingData.CompletionString, DxLib::GetColor(0, 255, 255)); // 入力済部分
		if (cMiss != '\0') { // ミスった文字があるなら描く
			TCHAR acNG[2]; acNG[0] = cMiss; acNG[1] = '\0';
			DxLib::DrawString(32 + DxLib::GetDrawStringWidth(TypingData.CompletionString, (int)DxLib::strlenDx(TypingData.CompletionString)), 96, acNG, DxLib::GetColor(255, 0, 0));
		}
		if (DxLib::ScreenFlip() != 0) break;
		if (TypingData.CompleteIndexNum == TypingData.TableIndexNum) { // 終わったか
			::MessageBox(NULL, TEXT("完了"), TEXT("APP"), MB_OK);
			break;
		}
	}

	return DxLib::DxLib_End();
}
