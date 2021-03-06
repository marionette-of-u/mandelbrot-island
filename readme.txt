--------------------------------
プログラム名 : Mandelbrot Island
バージョン   : ver 0.3.0 (2015/09/17)
製作者       : marionette-of-u
--------------------------------

---------------- 概要
マンデルブロ集合を描画します

---------------- 動作環境
CPU : できるだけ高速で、できるだけコア数の多いもの
MEM : できるだけ容量の多いもの
OS  : Windows 8.1 or later

---------------- その他
mpir.dllを自分のCPUの世代に合ったものでビルドすると
処理速度が速くなるかもしれない

---------------- ライセンス
mpir.dll - LGPL 2.1
それ以外のファイル - MITライセンス
生成されたファイル - プログラムを利用した人のご自由にどうぞ

---------------- GUI表示系
x - 現在描画されている実数値の区間
y - 現在描画されている虚数値の区間
z - 現在描画されている区間のズーム率
p - 浮動小数点数の精度。低い程低負荷
n - 色数。低い程低負荷。精密に描画したい場合に増やすと有効

---------------- GUI制御系
プレビュー画面
 - 好きな場所をD & Dで拡大
   ※何もない所をD & Dしても意味がありません！

function in { sin, cos, tan }
 - 左にあるものから順に、赤、緑、青
   着色する関数をクリックする事で選択

3つのシークバー
 - 上から順に、赤、緑、青を着色する関数に与える値ω
   D & Dで調整

---------------- キー制御系
[A] - 初期座標描画
[T] - キャッシュクリア。アンドゥできなくなります
[G] - データをsave.txtにセーブ。常に上書き保存します
[B] - データをsave.txtからロード。失敗した場合は初期座標が描画されます
[Z] - 現在の設定で描画
[X] - アンドゥ
[W] - 発散しない点を黒で描画する/しないのスイッチ
[S] - 描画形式をスムース
      (iter - (log_{2})^2 |z|) / maxiter
      に切り替えるスイッチ
[Q] - 処理中の状態を描画するかどうかのスイッチ。デフォルトでOFF
[X] - 処理を強制的に中断。処理中のみ有効な機能

bmp画像として出力
  [0] - 1/4倍
  [1] - 1倍
  [2] - 4倍
  [3] - 9倍
  [4] - 16倍

浮動小数点数の精度
  [E] - 減算
  [R] - 加算

色数
  [D] - 減算
  [F] - 加算

---------------- Special Thanks!
（順序不同）
OREO様
海野まぐお様
白茶利休様
TINA様
sbwhitecap様
