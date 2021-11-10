# librencodeplus

A recreation of XPRA's lib-rencode-plus library in C++.

Notes to myself:
- generate the .h headers:
`% cat rencodeplus-cpp.cpp| grep "^[^ ].*{$" | sed 's/{/;/g'`
