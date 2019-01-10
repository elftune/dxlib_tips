# DxLib Tips
ＤＸライブラリ(以下DxLib)でプログラミングする際、公式マニュアルだけではわかりにくい（非公開関数を使用するケースが多い）点がしばしば見られます。自分でもよく忘れるのでメモとして残します。

---

## VRM(.vrm)をPMX(.pmx)に変換してMMDやDxLibで使うための手順

VRoid Studio v0.5.0での説明。VRoid StudioやUnity用パッケージが頻繁にアップデートされるので仕様がころころ変わり、その結果もころころ変わるので
すぐに陳腐化するとは思いますが、まぁ念のため。  

VRoid StudioからVRoid.vrmでエクスポート  
↓  
UnityにUniVRMとVRMtoPMXExporterのpackageを読み込ませる  
↓  
UnityにVRoid.vrmを読み込ませる  
↓  
できたVRoid.prefabをHierarchyにDnDする  
↓  
PMXExporterにあるPMXExporter.csをDnDしたprefabにDnDする  
↓  
HierarchyでVRoidをクリックして選択状態にする  
↓  
Inspectorの下の方にPMX Exporter (script)ができている  
↓  
小さい歯車をクリックして表示される項目の一番下にExportがあるのでVRoid.pmxで書き出す  
↓  
VRoid.TexturesフォルダをVRoid.pmxのあるフォルダにコピーし、texフォルダにリネームする  
↓  
PmxEditorでVRoid.pmxを開く  
↓  
材質のうえのへんにある 3000_VRM+MToon(5)～5000_VRM+MToon(7)を選択して底ボタンをクリックして表示順位を下げる  
（こうしないと目の周りの半透明描画がダサい）  
↓  
ここまでで、MMDでまぁそれなりに表示できるPMXができた。  
しかし、DxLibで読み込んでも、足がぐにゃーんとなってまともに表示できない。  
↓  
PmxViewの方でCtrl-Aをして全ての頂点を選択  
↓  
プラグインでウェイトを全て書き出し(ウェイト.txt)  
↓  
ウェイト.txtを以下のように編集 (ここに置いた Replace.MAC を使うとよいだろう)  
・J_Adj_R_LowerLegとJ_Sec_R_LowerLegを右ひざに置換  
・J_Adj_R_UpperLegを右足に置換  
・左も同様に置換  
↓  
全てのウェイトを初期化（※しなくてもいいかもだが）  
↓  
プラグインでウェイトを上書き  
  
VRoidから出力すると一般的なPMXモデルとサイズが異なる場合が多いので、PmxEditorで調整すること。  
これで、DxLibでも足は正常に表示できるPMXができた。あとは以下の修正を行う。  
  
・髪の毛などの色が違う⇒なんとかしろ。  
・陰影が無い⇒PmxEditorで適当なtoonXX.bmpを使用する。  
・髪の毛がだらーんと垂れたり暴れる⇒髪の剛体とJointをいったん全削除する。髪のボーン名が判別不明なので前髪1とかに置き換える。まぁ各部分の根本だけでよいだろう。で、選択ボーン⇒基礎剛体/連結Jointの作成 でひたすら剛体とJointの組みあわせを作る。剛体は物理＋ボーンにしておく。できた（正確には作りかけの）Jointを 部位A --> 頭 ---> 部位A　の構成になるようにひたすら作る。これで、少しはましな挙動にはなる・・・のだが希望する動きではないのでいろいろいじろう。  
・モデルによっては腕やスカートの透明テクスチャが適切に処理されない⇒VRoid StudioのデフォルトモデルのうちVitaはスカートや腕の周りに透明ピクセルのあるテクスチャが使用されているが、DxLibはデフォルトでは普通に描画してしまう。このため「色は何もないが向こうが見えない」ような変な状態になる。Zバッファに描画されるため、その奥に描けないという問題が起きる。ということで該当する材質MToon(11)の2つについて、PmxEditorでエッジを無効にしてアルファを0にする。んで、描画側ではDxLib::MV1SetMaterialDrawAlphaTest(nHandle, 材質番号, TRUE, DX_CMP_GREATEREQUAL, 16とか適当 ) により材質によって描き分ける。MV1SetMaterialDrawAlphaTestAllで全材質に設定してもまぁいいのだが、やはり負荷を下げるため必要に応じて使い分けるのがよいだろう。bPではモデルごとに材質（と閾値になるアルファ値）を指定できるようにしてある。  



ちなみにDxLib Model Viewerでの読み込みオプションは、  
・法線再計算 をON  
・PMD/PMX読み込み時の物理演算のFPS を120  
にしておくこと。    

## DxLibのソースファイル自体を含めてビルドする
DxLibをしばらく使っていると、「これはどういう処理をしているのだろう」とか「もしかしたらライブラリのバグかもしれない」とかで、ライブラリ自体をソースレベルでビルドし、デバッグ実行してブレークポイントをライブラリ側にセットして挙動を確認とかがしたくなります（よね？）。

ライブラリのソースは配布されていますがこれは DxLib.lib を自分でカスタムするとかのためであって、ちょっと違う。ということで、DxLibのライブラリごとプロジェクトに含めてデバッグする方法を（自分のために）残しておきます。（DxLib自体の再コンパイル、再ビルド、リビルド、デバッグ）

DxLibは「正式版」と「Temp版」があります。今回は3.19d正式版と2018/10/12時点のTemp版を使用します。

構築するフォルダを用意する。今回は　E:\DX3　を使用。開発環境はVisual Studio 2017、Windows 10 x64 Pro。

http://dxlib.o.oo7.jp/dxdload.html から正式版をダウンロード (VC++用 3.19 (186MB) DxLib_VC3_19d.zip)  
ページの一番下【改造希望の方へ】の　DxLibMake3_19d.zip 4.68MB　もダウンロード。  
DX3フォルダに置き、7z等でここに展開する。  
E:\DX3\DxLib_VC  
E:\DX3\DxLibMake  

Temp版をダウンロードする。今回は関係ないが、DxLib Model Viewerも機能が最近追加されたのでGETしておく。  

http://dxlib.o.oo7.jp/temp/DxLibVCTest.zip

http://dxlib.o.oo7.jp/temp/DxLibMakeTest.zip

http://dxlib.o.oo7.jp/temp/DxLibModelViewerTest.zip


DxLibVCTest.zipとDxLibMakeTest.zipもDX3フォルダの下に展開する。  
E:\DX3\DxLibMakeTest の中身全部を E:\DX3\DxLibMake に上書きする。  
E:\DX3\DxLibVCTest の中身全部を E:\DX3\DxLib_VC\プロジェクトに追加すべきファイル_VC用 に上書きする。  
DxLibVCTest と DxLibMakeTest フォルダは削除する。  

E:\DX3\DxLibMake にある DxLibMake.sln を起動（他にも.sln があるので間違えないように）  
.sln のバージョンやツールセットをアップグレードせよと表示されるので OK を押す。  

F7 を押してビルド。少し時間はかかるが成功する。  
E:\DX3\DxLibMake\Debug に DxLib_d.lib ができた。  
…が、LIBを作るのではなく実行ファイルを作るのが目的なので、ここからが本番。  


ソリューションエクスプローラーから 追加 ⇒ 新しい項目 ⇒ C++ファイル(.cpp)　でプログラム用のファイルを用意。  
ファイル名はとりま Main.cpp にする（なんでもいい）。追加ボタンを押すとまっさらなファイルができるので  

```C++:
#include "DxLib.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	DxLib::ChangeWindowMode(TRUE);
	DxLib::DxLib_Init();
	DxLib::SetDrawScreen(DX_SCREEN_BACK);
	unsigned int i = 0;
	while (!DxLib::ScreenFlip() && !DxLib::ProcessMessage() && !DxLib::ClearDrawScreen()) {
		DxLib::DrawFormatString(0, 0, DxLib::GetColor(255,255,255), TEXT("カウント＝%d"), i++);
	}
	return DxLib::DxLib_End();
}
```

と適当に書く。（ちなみに DxLib:: をわざわざつけるのは C# に移植する時（少しは）楽だからとかそういう理由）

また、自分は基本的に x64・Unicode 用で開発するため、ここも同じスタンスにしている。  
ファイル ⇒ 名前を付けて保存で 上書き保存ボタンの右の ▼ をクリックし、エンコード付きで保存 ⇒ Unicode (UTF-8 シグネチャ付き) - コードページ 65001 で保存する。

プロジェクト ⇒ DxLibMake のプロパティから、構成マネージャー ⇒ アクティブソリューションプラットフォームを、  
Win32から 新規作成 に変更し、ARM⇒x64 にさらに変更する。（以降、x64 Debug のみで説明）

残りのプロパティも設定する。

|症状         |原因                      |
|:------------|:------------------------|
|構成の種類|スタティックライブラリ (.lib)⇒アプリケーション(.exe)|
|文字セット|マルチバイト文字セットを使用する⇒Unicode 文字セットを使用する<br>ここで適用ボタンを一回押しておく（※押さないと リンカー の項目が出ないので）|
|C/C++ ⇒ プリプロセッサ|WIN32;_DEBUG;_LIB;%(PreprocessorDefinitions) ⇒ _WIN64;WIN32;_DEBUG;_WINDOWS;%(PreprocessorDefinitions)|
|リンカー ⇒ 全般 ⇒ 追加のライブラリ ディレクトリ|E:\DX3\DxLib_VC\プロジェクトに追加すべきファイル_VC用　を追加|

F7を押してビルドする。しばらく待つと、リンクエラーが1665個発生。正常です（白目）

ソリューションエクスプローラーで 追加 ⇒ 既存の項目　から  
E:\DX3\DxLibMake　にある DxMemImgDrawFunction0.cpp ～ DxMemImgDrawFunction3.cpp の4つと、DxUseCLib.cpp、DxUseCLibOgg.cpp、DxUseCLibPhysics.cpp の3つを追加。

ビルドするとエラーが3つに減った！　というかエラー1662個がOKになったわけではない。「ファイルがないよ」ということでコンパイルが異常終了しているのである。これから、Vorbisなど外部ライブラリをGETすることになる。

忘れないうちに、ソリューションエクスプローラーで 追加 ⇒ 既存の項目　から  
E:\DX3\DxLibMake\Windows　にある DxUseCLibOggD3D9.cpp、DxUseCLibOggD3D11.cpp、DxUseCLibOggWin.cpp、DxUseCStrmBaseFilter.cpp　も追加しておく。

いったんVisual Studio は終了。ファイルをGETしていこう。


E:\DX3\DxLibMake に「DxUseCLib スタティックリンクライブラリについて.txt」というファイルがあり、そこにURL等は書いてあるので、ここでは補足的なことだけ書く。

|ファイル                                          |バージョンとかファイル名とか|
|:------------------------------------------------|:-------------------|
|Bullet Physics|後述の理由により、Ver.2.75一択。しかし https://pybullet.org/wordpress/ を見ても 2.8系しかない。<br>検索すると https://code.google.com/archive/p/bullet/downloads?page=3 にあった。bullet-2.75.zip をGET|
|libpng|1.6.35 (lpng1635.zip)|
|zlib|1.2.11 (zlib1211.zip)|
|libjpeg|9c (jpegsr9c.zip)|
|libtiff|4.09 (tiff-4.0.9.zip)|
|libopus|1.2.1 (opus-1.2.1.tar.gz)|
|opusfile|0.11 (opusfile-0.11.tar.gz)|

「DxUseCLib スタティックリンクライブラリについて.txt」は古いので、Opus Codec について書かれていないので注意。  
上記の表のラスト2つがそれ。https://opus-codec.org/downloads/ からGETしよう。

E:\DX3\EXT_LIB フォルダを作り、とりあえずファイルをここに置く。.tar.gz は .tar に展開しよう。

bullet-2.75.zip  
jpegsr9c.zip  
lpng1635.zip  
opus-1.2.1.tar  
opusfile-0.11.tar  
tiff-4.0.9.zip  
zlib1211.zip  

が集まったので、それぞれフォルダに展開する。
E:\DX3\EXT_LIB\bullet-2.75  
E:\DX3\EXT_LIB\jpeg-9c  
E:\DX3\EXT_LIB\lpng1635  
E:\DX3\EXT_LIB\opus-1.2.1  
E:\DX3\EXT_LIB\opusfile-0.11  
E:\DX3\EXT_LIB\tiff-4.0.9  
E:\DX3\EXT_LIB\zlib-1.2.11  

といったところだろう。

次に、Ogg, Vorbis, Theora をGETする。  
E:\DX3 の下に Vorbis, Ogg, Theora のフォルダをそれぞれ作っておく。

SVNクライアントソフトを持っていなかったら、TortoiseSVN をインストールする。  
「DxUseCLib スタティックリンクライブラリについて.txt」の最後あたりにリポジトリが書いてあるのでそれを使う。  
まず、Vorbis フォルダを右クリックし、メニューから SVN Checkアウト を選ぶ。  
URL of repository に http://svn.xiph.org/trunk/vorbis と入力して OK を押すとファイルがDLされる。  
Ogg, Theora も同様にする。

ダウンロードはここまで。さて、Bullet Physics は 2.75一択 という説明をしただが、詳しくは「DxUseCLib スタティックリンクライブラリについて.txt」に書いてある。要は、Bulletのクラス名等にごっそりと D_ をつける必要があるため。  
当然手作業ではやってられないので、これを実行できるプログラムが用意されているのだが、これが 2.75専用 なのである。

さて「これを実行できるプログラム」と書いたがが、なぜかソースだけであり各自でビルドしなければならない。  
Win32 コンソールアプリを作ってやればいいのだが、ある意味ここが一番難しいかもしれない。  
__pragma(warning(disable:4996))　を追加したり、一か所どっかエラーになるのを直したりする必要がある。  
各自頑張ってほしい（手抜き）。ここでは BulletChange.exe というファイル名を作成したとする。  

ビルド出来たらDOS窓から BulletChange.exe E:\DX3\EXT_LIB\bullet-2.75\src と実行し、しばらく待つと変換が完成する

E:\DX3\EXT_LIB\bullet-2.75\src\LinearMath　にある btScalar.h　のバックアップを取っておく。
btScalar.h をテキストエディタで開き、  
D_SIMD_FORCE_INLINE D_btScalar D_btSqrt(D_btScalar y)   
関数の数行目にある　/* hoist out the の後が文字化けしているので、削って　/* hoist out the 1/2 */　とでもしておく  
(Shift-JISでは表示できない文字が入っている。秀丸で欧文フォーマットで開くと読めるが、特に必要ないので消しておく）

E:\DX3\EXT_LIB\jpeg-9c にある jconfig.vc を jconfig.h にリネームする。  
E:\DX3\EXT_LIB\lpng1635\scripts にある pnglibconf.h.prebuilt を pnglibconf.h にリネームしてE:\DX3\EXT_LIB\lpng1635 に置く。  
E:\DX3\EXT_LIB\tiff-4.0.9\libtiff にある tiffconf.vc.h を tiffconf.h にリネームする。  
E:\DX3\EXT_LIB\jpeg-9c にある jconfig.vc を jconfig.h にリネームする。


ここから Visual Studio 2017 に戻る。  
DxLibMake.sln を開き、プロジェクト ⇒ DxLibMake のプロパティ ⇒ C/C++　追加のインクルード ディレクトリ に

E:\DX3\EXT_LIB\opusfile-0.11\include;  
E:\DX3\EXT_LIB\opus-1.2.1\include;  
E:\DX3\EXT_LIB\bullet-2.75\src;  
E:\DX3\EXT_LIB\jpeg-9c;  
E:\DX3\EXT_LIB\lpng1635;  
E:\DX3\EXT_LIB\tiff-4.0.9\libtiff;  
E:\DX3\EXT_LIB\zlib-1.2.11;  
E:\DX3\EXT_LIB\Vorbis\include;  
E:\DX3\EXT_LIB\Vorbis\lib;  
E:\DX3\EXT_LIB\Theora\include;  
E:\DX3\EXT_LIB\Ogg\include;  

を追加する。Vorbis は2つあるので注意。

E:\DX3\EXT_LIB\opusfile-0.11\include;E:\DX3\EXT_LIB\opus-1.2.1\include;E:\DX3\EXT_LIB\bullet-2.75\src;E:\DX3\EXT_LIB\jpeg-9c;E:\DX3\EXT_LIB\lpng1635;E:\DX3\EXT_LIB\tiff-4.0.9\libtiff;E:\DX3\EXT_LIB\zlib-1.2.11;E:\DX3\EXT_LIB\Vorbis\include;E:\DX3\EXT_LIB\Vorbis\lib;E:\DX3\EXT_LIB\Theora\include;E:\DX3\EXT_LIB\Ogg\include;%(AdditionalIncludeDirectories)

こんな感じ。  
いつも思うのだが、Visual Studio のフォルダ指定ダイアログとかテキストボックスとか、いつまでこんな狭い状態で使わせるのだろうかね。

さて、これで終了。ビルド・実行もOK（のはず）。


## ストリーミングモードで音声ファイルを読み込む


```C++:
int SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE); // ストリームモードで開く

int SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS); // 通常読み込みモードに戻す
```
CPU負荷もそれなりに増えるため、なんでもかんでもストリームで、というのも考え物でしょう。  
（デフォルトは通常読み込みモードです）


## 読み込んだGRAPHが静止画か動画かを判別する

[参考：公式BBS] (http://dxlib.o.oo7.jp/cgi/patiobbs/patio.cgi?mode=view&no=4220 "読み込んだ静止画・動画の判別について")

DxLibは静止画でも動画でも気にすることなく`LoadGraph()`で読み込んでくれますが、それが静止画なのか動画なのかを知りたいことがあると思います。リズムゲームでよく使用されるスクリプト`BMS`の世界では、スクリプト上`BGA.bmp`と書かれていても実際に存在するファイルは`BGA.jpg`で、でも中身はMPEG1だったりすることがあるため、拡張子はさっぱりアテになりません。  
本来の用途ではありません（と思います）が、以下の関数の戻り値で判別できます。

```C++:
int GetLastUpdateTimeMovieToGraph(int GraphHandle);
// 動画ファイルのイメージを最後に更新した時間を得る(ミリ秒単位)

戻り値が -1 なら静止画、0(以上)なら動画
```

## Zバッファ深度設定は1つだけではない

[参考：公式BBS] (http://dxlib.o.oo7.jp/cgi/patiobbs/patio.cgi?mode=view&no=4296 "縮小描画の注意点について")

DxLibはMMDモデル(*.pmx, *.pmd)を(制限はありますが)簡単に表示できます。  
デフォルトではZバッファの深度は16bitですが、ポリゴンやテクスチャの表示が乱れる(モアレ状になったり)ことが多く、24bitに設定する必要が多いかと思います。

```C++:
int SetZBufferBitDepth(int BitDepth);
// 画面用のＺバッファのビット深度を設定する( 16 or 24 or 32 )

int SetCreateDrawValidGraphZBufferBitDepth(int BitDepth);
// SetDrawScreen の引数として渡せる( 描画対象として使用できる )グラフィックハンドルに
// 適用するＺバッファのビット深度を設定する( BitDepth:ビット深度( 指定可能な値は 16, 24, 32 の
// 何れか( SetDrawValidGraphCreateFlag// 関数で描画対象として使用できるグラフィックハンドルを
// 作成するように設定されていないと効果ありません )
```

前者の関数はそのまんまなのでわかりやすいですし、普通はそれだけでよいのですが、バックバッファとは別にオフスクリーンを`MakeScreen()`で作成し、そこに描画する場合には、後者の関数も実行しないとオフスクリーンのZバッファ深度が16bitのままになります。



## SetWindowSizeで思い通りのサイズ変更ができない場合

[参考：公式BBS] (http://dxlib.o.oo7.jp/cgi/patiobbs/patio.cgi?mode=view&no=3696 "SetHookWinProcについて by kiaiさん")

実行直後にウィンドウのサイズ（というかクライアントエリアのサイズ）を、`SetWindowSize()`によって1280x720⇒640x360にサイズ変更するだけのコードですが、これはうまくいきません。

```C++:
#include "DxLib.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::DefWindowProc(hWnd, message, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	ChangeWindowMode(TRUE);
	SetGraphMode(1280, 720, 32);
	if (DxLib_Init() < 0) return -1L;
	SetDrawScreen(DX_SCREEN_BACK);
	int nGID = LoadGraph(L"Title.jpg");
	SetHookWinProc(WndProc);

	while (!ProcessMessage()) {
		ClearDrawScreen();
		DrawGraph(0, 0, nGID, FALSE);
		ScreenFlip();
		SetWindowSize(640, 360); // サイズ変更！
	}

	DxLib::DxLib_End();
	return 0;
}
```

サイズは確かに変わるのですが、クライアントエリアの周辺に変な隙間ができてしまいます（ゴミも表示される）。  
DxLibは`SetHookWinProc()`によりWndProcを設定できます。上記コードでも設定していますが、実はWndProc中の`return`の部分に問題があります。ここは

```C++:
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}
```

でいいとのこと。DxLibでもメッセージ処理を実施しているため0を返してくださいとのことです。


## ループ情報付きWAVEファイルを無効にする

[参考：公式BBS] (http://dxlib.o.oo7.jp/cgi/patiobbs/patio.cgi?mode=view&no=4232 "wavファイルのループ情報 by BayLeafさん")

BMS PLAYERを作っているにもかかわらず音楽の知識ZEROなのですが、WAVEファイル`(*.wav)`にはループ情報を含めることができます（知らんかった）。  
BMSで使用されるメディアのフォーマットのうち、近年では音声ファイルはOgg Vorbis`(*.ogg)`がほとんどですが、たまにWAVEもあります。  
で、なぜかループ情報が設定されているWAVEがたまにあったりします。

```
※BMS PLAYERの標準と言える Lunatic Rave 2 (LR2) でもそのWAVEはループ再生しませんので、ループしないで正解（のはず）。
```

DxLibではOgg Vorbisも簡単に扱えます（※MPEG1 Audio Layer III`(.mp3)`はx64環境でコーデックの問題によるものなのか、読み込みが非常に遅かったので個人的にはお勧めできません）が、もちろんWAVEも読み込めます。  
しかし、「ループ情報付きWAVEをそのまま読み込んだ場合、その音声データは必ずループする仕様になり、1回だけ鳴らして止めることはできない。対応しようとするとライブラリの大改造が必要になる。」とのことです。

ただ、読み込む際に「ループ情報を無視するモードに切り替える」関数が追加されたため、それを実行すればOKになっています。

```C++:
int SetCreateSoundIgnoreLoopAreaInfo(int IgnoreFlag);
// LoadSoundMem などで読み込むサウンドデータにループ範囲情報があっても無視するかどうかを
// 設定する( TRUE:無視する  FALSE:無視しない( デフォルト ) )
```

## DirectX11とDirectX9の相違点

[参考：公式BBS] (http://dxlib.o.oo7.jp/cgi/patiobbs/patio.cgi?mode=view&no=4063 "最近のDxLibのバージョンでの不具合")

DxLibはDirectX9やDirectX11の違いを特に気にせずプログラミングすることができます。デフォルトでは、最近のPCであれば通常はDirectX11モードで動作するプログラムになります。  
しかし、長らく私は下記のようにわざわざDirectX9モードで動作するように設定していました。

```C++:初期化あたり
	SetUseDirect3DVersion(DX_DIRECT3D_9); // DirectX9モードを指定
	SetZBufferBitDepth(24);
	if (DxLib_Init() == -1) return -1L;
```

なぜかというと、デフォルト、つまりDirectX11モードで開発している時に

- MMDモデルのテクスチャがモアレ状に乱れる
- GRAPHが描画されない場合がある

という症状が出て困っていたからです。DirectX9モードでは何も問題なかったため、「せっかくDirectX11対応のビデオカードなのにもったいないなぁ。みんな、同じような症状出ていないのかな？」と思いながらプログラムしていました。

ただ、これには原因がありました。

|症状         |原因         |
|:------------|:------------|
|MMDモデルのテクスチャがモアレ状に乱れる|上記コードで、`DxLib_Init()`の前に`SetZBufferBitDepth()`を呼んでいるから。DirectX9ではたまたま問題なかっただけ。|
|GRAPHが描画されない場合がある|バックバッファをクリアするとき、`ClearDrawScreen(&rc)`のように部分的にクリアしていたため。<br>DirectX9ではこれは有効だったが、DirectX11では仕様変更により「全体クリア」しかない（DxLibで、DirectX11ではrcは指定しても意味をなさないようになっている。そもそもこの引数は隠し引数だったので、今後も残るかは保証できないとのこと）ため、バックバッファのクリアが意図したものと異なり、結果として描画がおかしくなっていた。|

ということでした。  
やはり、安易に「ライブラリのバグ」となどと思ってはいけませんね！


## 動画フォーマットについて

[参考①：公式BBS] (http://dxlib.o.oo7.jp/cgi/patiobbs/patio.cgi?mode=view&no=4132&p=2 "DxLibで再生可能な動画の形式について by Citrineさん")

[参考②：公式BBS] (http://dxlib.o.oo7.jp/cgi/patiobbs/patio.cgi?mode=view&no=4264 "3.19版でLoadGraphを用いたWMV動画再生 by ひゅうがさん")

DxLibはDirectShowを使用した動画再生に対応していますが、個人的にはDxLibがDirectShowではなくネイティブで対応しているOgg Theoraを使用しています。と言っても「DirectShowだとCodecの有無や種類によって再生できないとか、再生タイミングがずれる」等の問題によるものではなく、「DxLibでは、OGVでないとシークできない」という点によるものです。

ただし、現在では上記「[参考①：公式BBS] (http://dxlib.o.oo7.jp/cgi/patiobbs/patio.cgi?mode=view&no=4132&p=2 "DxLibで再生可能な動画の形式について by Citrineさん")」にあるように、H.264/AVC (*.mp4) にも対応していただきましたので、調達のしやすさからH.264に移行しつつあります。（ちなみに、シーク速度はH.264の方が速い）

…が、上記BBSにも書いたようにDxLibにおけるH.264は

- OGVより重い
- 使用メモリが（異様に）増え続ける

という問題があります。再生が終わればメモリは解放されますし、BMSでは動画は長くても3分程度ですのでメモリ増加量は特に問題はありませんが、長い動画を再生するアプリケーションではどうなるかはなんとも言えません。

OGV形式のファイルを作成するには、任意の動画を `ffmpeg2theora` でエンコードするか、`TMPGEnc Video Mastering Works 6` でエンコードするくらいでしょうか。ですが、`ffmpeg2theora` の最新バージョン`0.30`はどうもうまく動作しないので`0.29`を使わざるを得ないように思います（`0.29`でもエラーがたまにでますが…）し、`TMPGEnc Video Mastering Works 6`はH.264の作成では全く問題ありませんが、OGVの作成ではフレームが抜けたりする変なバグがあり、安心して使用できませんでした（最近のアップデートでどうなったかはわかりませんが、2015年後半~2017年前半で解消しなかったのでそれから試していない）。

ちなみにOGVを再生するには`VLC Media Player`などをインストールすればOKですが、`Firefox`がインストールされているなら、タブ部分付近に放り込めば再生できます。(OGGも同様)

```
「動画再生はMPEG1が最も悩まなくて済む」という記述をよく見ますが、日本、韓国、(おそらく)台湾の方が
それぞれ作成したMPEG1ファイルを、すべて正常に再生できるコーデックを見つけきれなかったという経験が
あるので、MPEG1はむしろ個人的に最も苦戦するコーデックに位置付けています。
```


## 64bitビルドのすすめ

DxLibを使い始めたのは2015年11月頃で、当初、公式マニュアルにあるように32bit(x86)で実施していましたが、動作確認しているとどうもテクスチャが抜けたり、メディアファイルの読み込みエラーが出たり、挙動が怪しくなることが増えました。要は32bitアプリなのでメモリ不足になっており、64bitアプリにすればOK、ということで試してみました。

確かに挙動は安定しました。とはいえ、MMDを2キャラ＋BMSの画像・音声でそこまでメモリを食うのだろうか？という疑問もあります。（検証のため、MMDをOFFにしても同様の症状になったので）  
DxLibのマニュアルでは「MMDモデルの管理などでメモリを結構食います」とありますので、まぁ変なところで悩むくらいなら、最初からx64でビルドしていくのがよいと思います。幸い、現在は64bit OSの割合が高いと思われますので。

ただ、上の方で描きましたが、x64に変更することでいくつか注意すべきこともでてきます。(MP3のコーデックとか)

```
なおBMS PLAYERという性格上、韓国語の表示も必須と言えますので、MBCSではなくUnicodeで組んでいます。
```
