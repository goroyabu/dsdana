# Data Format

## File Format

- デフォルトの入出力はROOTファイルとROOTクラスのTTreeを使用する.
- 入力TTree : eventtree
- 出力TTree : hittree

現在、出力したhittreeがhaddできません。原因は調査中です。すみません。

## Branch of hittree
- ヒット(入射光子が検出器内に落としたエネルギー値とその位置)のリスト
- 1エントリー=1ヒット

### eventID / int
- イベント番号
- 1読み出しに2ヒットある場合は同じeventIDが振られる

### detid / int
- 検出器番号

### epi / float
- ヒットのエネルギー

### epi_x / float
- Pt-side(DSSDならp-side)のエネルギー

### epi_y / float
- Al-side(n-side)のエネルギー

### pos_x / float 
- ヒットのX位置

### pos_y / float
- ヒットのY位置
 
### pos_z / float
- ヒットのZ位置

### event_status / int
- イベントの状態
- 数字とステータスの対応づけはドキュメントにしておこう

### unixtime, livetime... などの時間情報
- eventtreeの項を参照


## Branch of detailed hittree 
- 詳細な情報を含むhittreeの説明
- 解析をはじめたばかりの人はとりあえずスルーでOK
- 1エントリー=1読み出し

-
### Lv1 Branch

#### nsignal_x_lv1 / int
- A number of signals on Pt-side.
- 'nsignal_y_lv1' for Al-side.

#### detid_x_lv1[nsignal_x_lv1] / int
- A detector ID defined in the database file.
- As default, 'detid' is 0 for a single layer detector.

#### stripid_x_lv1[nsignal_x_lv1] / int
- A strip ID defined in the database file.

#### epi_x_lv1[nsignal_x_lv1] / float
- A EPI value converted from PHA.


-
### Lv2 Branch

#### nsignal_x_lv2 / int
- A number of merged signals on Pt-side.
- 'nsignal_y_lv1' for Al-side.

#### detid_x_lv2[nsignal_x_lv2] / int
- A detector ID defined in the database file.

#### epi_x_lv2[nsignal_x_lv2] / float
- A EPI value after merging.

#### pos_x_lv2[nsignal_x_lv2] / float
- A position value after merging.

#### width_x_lv2[nsignal_x_lv2] / float
- A width value after merging.

#### n_merged_strips_x_lv2[nsignal_x_lv2] / float
- A number of strips in the signal after merging.

#### n_lv1signal_x_lv2 / int
- A total number of 'lv1signal_id_x_lv2'.

#### lv1signal_id_x_lv2[n_lv1signal_x_lv2] / int
- An array of indices of lv1 data used in lv2 data.
- You can access signals before merging with this.

 
-----
## Branch of eventtree
- 入力とするTTree(TChainも使用可)
- 以下では、本ソフトで使用するブランチについてのみ説明
- ブランチ名のXXはASICID。 e.g. adc0, adc1, adc2 ...   
   
#### adcXX[64] / unsigned short
- ASICから読み出されたADC値。
- 次のindexと照らし合わせることで、チャンネル番号との対応がわかる。
	
#### indexXX[64] / unsigned short
- 信号が読み出されたチャンネル番号。
- 同じ添字のadcと対応している。
	
#### hitnumXX / unsigned short
- ASICから読み出された信号の数。
- adc,indexはこの数まで読めばよい。
	
#### cmnXX / unsigned short
- ASICで計算されたCMN(コモンモードノイズ)。
- adcからcmnを差し引いた値をPHA値とする。


-
#### unixtime / unsigned int
- UNIX時刻. 単位はsecond(秒).

#### livetime / unsigned int
- 前のイベントと現在のイベントの間の時間.
- 単位はclock. 1 clock = 50 nsec.
	
#### integral_livetime / unsigned int
- これまでのlivetimeの積分値.
- J-PARC実験では, ビームラインからのトリガー情報を入れていた(紛らわしい).

#### ti / unsigned int
- 忘れた。


