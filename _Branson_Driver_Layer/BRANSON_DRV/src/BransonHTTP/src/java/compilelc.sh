
echo Compiling Live Control...
echo -------------------------

COMPILER=javac 
COMPILER_ARGS="-source 1.4"

#may have to change this to explicitly point to your webcli component directory
WEBCLI=${WRVX_COMPBASE}/${COMP_WEBCLI}

SRC=${WEBCLI}/target/src/wrn/wm/http/java
DEST=${WEBCLI}/target/src/wrn/wm/http/java/class

mkdir -p ${DEST}

cd ${SRC}/http/util
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} -d ${DEST} *.java

cd ${SRC}/http/livecontrol/comm
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} -d ${DEST} *.java
				     		     
cd ${SRC}/http/livecontrol/converter     		     
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} -d ${DEST} *.java		  
cd ${SRC}/http/livecontrol/common                    
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} -d ${DEST} *.java
			                     
cd ${SRC}/http/livecontrol/directevents	                     
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} -d ${DEST} *.java

cd ${SRC}/http/livecontrol/widgets	                     
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} -d ${DEST} *.java

cd ${SRC}/http/test
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} -d ${DEST} *.java
					     		     
cd ${SRC}/http/livecontrol/directevents    		     
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} -d ${DEST} *.java

cd ${SRC}/http/test   		     
${COMPILER} ${COMPILER_ARGS} -classpath ${DEST} -sourcepath ${SRC} *.java

echo ...done.
