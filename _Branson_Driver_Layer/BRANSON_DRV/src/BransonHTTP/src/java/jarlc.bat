rem === Create the LiveControl standard JAR file ===
cd class
jar cvf0 livecontrol.jar http
cd ..

cd classes
del livecontrol_old.jar
ren livecontrol.jar livecontrol_old.jar
copy ..\class\livecontrol.jar
cd ..
