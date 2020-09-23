# カメラ画像から散布物の分布を取得
画像のファイル名と散布物の情報（色と大きさ）を入力すると，散布不足の領域の座標を返す．

# class Camera
カメラあるいは指定したファイルのread,show,writeができる．
readする際にstring型の引数を2つ渡すと差分が与えられる．
writeするファイル名は入力したファイルのディレクトリ名と末尾を編集したものになる．

# class Objectfeature 
平滑化処理する際のマスの数，および，エリアの分割広さ，二値化のためにしきい値を格納する．
それぞれの散布物に対し，適切な処理のためにこれらの値を調整する．

# class distriburionCamera
class Cameraを継承している
散布対象の領域はコンストラクタで指定できる
filtering()で平滑処理と二値化を行っている．差分があったところは赤色に塗りつぶす．
judge()で各エリアに散布されているかどうかを判別する．散布されていない領域を緑色にする．
judge()の引数で散布すべきエリアの座量がvector型として取得できる．


# main
ファイル名を格納，散布物ごとにObjectfeatureのパラメータを設定している
実行するとモード選択になる
ターミナルから入力することで，全ての写真を判定して保存するか，特定のファイルの結果の表示をするかの選択が可能となる．
調整を行いたい場合には's'を入力して特定ファイルの選択を行えばよい．
キーボードに入力すると表示が停止できる．



