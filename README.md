# カメラ画像から散布物の分布を取得
画像のファイル名と散布物の情報（色と大きさ）を入力すると，散布不足の領域の座標を返す

# class Camera
カメラあるいは指定したファイルのread,show,writeができる
readする際にstring型の引数を2つ渡すと差分が与えられる
writeするファイル名は入力したファイルのディレクトリ名と末尾を編集したものになる

# class Objectfeature 
平滑化処理する際のマスの数，および，指定座標の分解能，二値化のためにしきい値を格納する
それぞれの散布物に対し，適切な処理のためにこれらの値を調整する．

# class distriburionCamera
class Cameraを継承している


