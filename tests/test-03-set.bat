@if exist "%~dp0\_setup_tests.bat" @call "%~dp0\_setup_tests.bat"
@echo OUTROOTPATH=%OUTROOTPATH%
@echo SLN_EXE=%SLN_EXE%
%SLN_EXE% --replace-sz --no-expand-sz --set=TEST2 test222