## 「言語処理系」の授業で使用したプログラムコード
ここでは、2015年に「言語処理系」(鹿児島高専/情報工学科/3年生向け)の授業の
際に使用したプログラムコードを提示しています。(自分の忘備録を兼ねています)

## コンパイル対象言語
### 簡単な文法G1
### PL/0'

## プログラムの説明

### [字句解析器](lexer)
* 「PL/0'」用字句解析器(lex版)

### [簡単な文法G1用構文解析器](exp-parser)
*  LL構文解析器
*   (LA)LR構文解析器(bison)

### [「PL/0'」用構文解析器](pl0-parser)
*  LL構文解析器
*  (LA)LR構文解析器(bison)

### [「PL/0'(最終版)」用コンパイラ](pl0-last-compiler)
どちらもLL構文解析器のみです。
*  関数未対応バージョン
*  関数対応バージョン

### 実行環境
授業で説明した簡易CPUの命令を解釈して実行します。

- [簡易CPUの命令体系](vcpu/instruction-set.md)

- [ryo33/pl0dashvm](https://github.com/ryo33/pl0dashvm) (Go言語による実装 By [ryo33](https://github.com/ryo33/))
- [NotFounds/pl0dash-run](https://github.com/NotFounds/pl0dash-run)
 (C#言語による実装 By [NotFounds](https://github.com/NotFounds/))
- [vcpu/pl0dashvm.rb](vcpu/pl0dashvm.rb)
 (ruby言語による実装 By 下園)
