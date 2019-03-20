  
#!/bin/bash
#
# Execute the generated code
#
# This script takes in the filename of the ORIGINAL program AFTER the codegen has been run
#   i.e. programs/3-semantics+codegen/valid/test.go
#
# It MUST then
#   (a) Compile the GENERATED file
#         i.e. programs/3-semantics+codegen/valid/main/test.java
#   (b) Execute the compiled code
#
#
# To conform with the verification script, this script MUST:
#   (a) Output ONLY the execution

rm ${1%.*}.java 2> /dev/null

# You MUST replace the following line with the command to compile your generated code
# Note the bash replacement which changes:
#   programs/3-semantics+codegen/valid/test.go -> programs/3-semantics+codegen/valid/main/test.java

FILENAMEWITHJAVA="$(dirname ${1%.*})/main/$(basename ${1%.*}).java"
javac $FILENAMEWITHJAVA

# You MUST replace the following line with the command to execute your compiled code
# Note the bash replacement which changes:
#   programs/3-semantics+codegen/valid/test.go -> programs/3-semantics+codegen/valid/main/test.java

cd "$(dirname ${1%.*})"
JAVAPATH="main/$(basename ${1%.*})"
java $JAVAPATH

# Lastly, we propagate the exit code
exit $?