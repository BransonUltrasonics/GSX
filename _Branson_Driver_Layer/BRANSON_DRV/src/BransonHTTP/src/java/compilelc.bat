@echo off

pushd .

echo Compiling Live Control...
echo -------------------------

set COMPILER=javac
set COMPILER_ARGS="-source 1.4"

set WEBCLI=%WRVX_COMPBASE%\%COMP_WEBCLI%

set SRC=%WEBCLI%\target\src\wrn\wm\http\java
set DEST=%WEBCLI%\target\src\wrn\wm\http\java\class

if not exist %DEST% md %DEST%

cd %SRC%\http\util
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% -d %DEST% *.java
		
cd %SRC%\http\livecontrol\comm
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% -d %DEST% *.java
				     		     
cd %SRC%\http\livecontrol\converter     		     
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% -d %DEST% *.java		  

cd %SRC%\http\livecontrol\common                    
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% -d %DEST% *.java
			                     
cd %SRC%\http\livecontrol\directevents	                     
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% -d %DEST% *.java

cd %SRC%\http\livecontrol\widgets	                     
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% -d %DEST% *.java

cd %SRC%\http\test
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% -d %DEST% *.java
					     		     
cd %SRC%\http\livecontrol\directevents    		     
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% -d %DEST% *.java

cd %SRC%\http\test   		     
%COMPILER% -source 1.4 -classpath %DEST% -sourcepath %SRC% *.java

echo ...done.

popd
