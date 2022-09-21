@mkdir .\build
@cd .\build

@cl ..\src\*.cpp /W0 /Fe"./compiler"
@del .\*.obj

@cd ..\
