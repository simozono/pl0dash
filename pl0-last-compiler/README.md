# 「PL/0'(最終版)」用コンパイラ
## ファイルの説明
### 共通ファイル
* [codegen.c](codegen.c) アセンブリ言語コード生成用関数群
* [codegen.h](codegen.h) 上記ヘッダーファイル(主にextern宣言)
* [symbol_table.c](symbol_table.c) 記号表関係関数群
* [symbol_table.h](symbol_table.h) 上記ヘッダーファイル(主にextern宣言)
* [misc.c](misc.c) エラー表示とかの関数群
* [misc.h](misc.h) 上記ヘッダーファイル(主にextern宣言)

### 別ディレクトリにあるファイル
* [../pl0-parser/pl0-scanner.l](../pl0-parser/pl0-scanner.l) 字句解析用定義ファイル(lexファイル)

### コンパイラ本体
* [pl0-compiler01.c](pl0-compiler01.c) 関数未対応バージョンの完成版
* [pl0-compiler02.c](pl0-compiler02.c) 関数対応バージョンの完成版

### コンパイラテスト用PL/0'サンプルコード

#### 関数呼び出しなし

* [sample/test00.pl0](sample/test00.pl0)

   符号のテスト
    ````
    var x, y;
    begin
      x := 1;
      x := -x;
    end.
    ````
* [sample/test01.pl0](sample/test01.pl0)

    単純なコード
    ````
    begin
      write 1;
      writeln;
    end.
    ````
* [sample/test02.pl0](sample/test02.pl0)

    定数/変数宣言と参照、四則演算および代入
    ````
    const n = 10;
    var x;

    begin
      x := n + 3;
      write x * 2;
      writeln;
    end.
    ````
* [sample/test03.pl0](sample/test03.pl0)

    if文
    ````
    const n = 3;
    var x;
    
    begin
      x := 9;
      if x > n then
      begin
        x := 1;
        write 3;
      end;
      write x;
      writeln;
    end.
    ````
* [sample/test04.pl0](sample/test04.pl0)

    while文
    ````
    const n = 10;
    var x;

    begin
      x := 1;
      while x <= n do
      begin
        write x * x; writeln;
        x := x + 1;
      end;
      write 1; writeln;
    end.
    ````

#### 関数呼び出しあり

* [sample/test101.pl0](sample/test101.pl0)

    3引数関数
    ````
    const n = 10;
    function func1(x, y, z)
    begin
      x := x + 1;
      return x+y+z;
    end;

    begin
      write 1+func1(1+2, 3+4, 5+6);
      writeln;
    end.
    ````
    
* [sample/test102.pl0](sample/test102.pl0)

    関数に return 文なし
    ````
    const n = 10;
    function func1(x, y, z)
    begin
      x := x + 1;
      write x+y+z;
      writeln;
    end;

    begin
      write 1+func1(1+2, 3+4, 5+6);
      writeln;
    end.
    ````
