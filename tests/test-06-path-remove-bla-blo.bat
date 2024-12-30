@if exist "%~dp0\_setup_tests.bat" @call "%~dp0\_setup_tests.bat"
@echo OUTROOTPATH=%OUTROOTPATH%
@echo SLN_EXE=%SLN_EXE%
%SLN_EXE% --path-remove BLO --path-remove=BLA
