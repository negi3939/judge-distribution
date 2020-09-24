# カメラ画像から散布物の分布を取得
散布前の画像のファイル名と散布後の画像のファイル名をを入力すると，散布不足の領域の座標を返す．
![pizza_3_2](https://user-images.githubusercontent.com/50578550/94103076-9d788380-fe6e-11ea-8f69-0ef3db9d4be7.jpg)
![pizza_3_2_output](https://user-images.githubusercontent.com/50578550/94103390-8be3ab80-fe6f-11ea-83c4-d43cf850af7d.jpg)

# class Camera
カメラあるいは指定したファイルのread,show,writeができる．
readする際にstring型の引数を2つ渡すと差分が与えられる．
writeするファイル名は入力したファイルのディレクトリ名と末尾を編集したものになる．

# class Objectfeature 
平滑化処理する際のマスの数，および，エリアの分割広さ，二値化のためにしきい値を格納する．
それぞれの散布物に対し，適切な処理のためにこれらの値を調整する．

# class distributionCamera
class Cameraを継承している．
散布対象の領域はコンストラクタで指定できる．
写真でなく，カメラで行うことも可能．差分情報から散布物を識別する．
filtering()で平滑処理と二値化を行っている．差分があったところは赤色に塗りつぶす．
judge()で各エリアに散布されているかどうかを判別する．散布されていない領域を緑色にする．
judge()の引数で散布すべきエリアの座標がvector<point>型として取得できる．
judge()処理後にgopopint.at(jj).x，gopopint.at(jj).yでdouble型としてつかえる．

# main
ファイル名を格納，散布物ごとにObjectfeatureのパラメータを設定している
実行するとモード選択になる
ターミナルから入力することで，全ての写真を判定して保存するか，特定のファイルの結果の表示をするかの選択が可能となる．
調整を行いたい場合には's'を入力して特定ファイルの選択を行えばよい．
キーボードに入力すると表示が停止できる．