はじめに
========
MacTypeについてよく分かっていない人、
設定が固まっていない人は使用しないください。
MacTypeは同梱はしていません。必要なファイルは自分で用意してください。
SetWindowsHookExでMacType.dllをのフックをかけてるだけです。
でも、なにが起こってもnocd5は一切の責任を負いません。

インストール
============
以下をbbMacTypeと同じディレクトリに置いてください。
・MacType.dll
・EasyHK32.dll
・MacType.ini
・ini/<MacType.iniで指定されている設定ファイル>

既知の問題
==========
MacTypeをロードしてるプロセスを列挙してDLLを抜くコードがないので、
HookChildProcesses=1
とするとbbMacTypeをアンロードしても子プロセスのMacTypeが抜けません。
したがって上記の設定は使わないでください。
