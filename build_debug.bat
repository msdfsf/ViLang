@mkdir .\build\debug
@cd .\build\debug

@cl -Zi ..\..\src\*.cpp  /Fe"./compiler"

@cd ..\..\