  
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

# You MUST replace the following line with the command to compile your generated code
# Note the bash replacement which changes:
#   programs/3-semantics+codegen/valid/test.go -> programs/3-semantics+codegen/valid/main/test.java

EXECUTEPATH="$(dirname ${1%.*})"
JAVAFILEWITHDASHES="$(basename ${1%.*})"
JAVAFILE="${JAVAFILEWITHDASHES//-}"
javac $EXECUTEPATH/$JAVAFILE.java

# You MUST replace the following line with the command to execute your compiled code
# Note the bash replacement which changes:
#   programs/3-semantics+codegen/valid/test.go -> programs/3-semantics+codegen/valid/main/test.java

cd $EXECUTEPATH
java $JAVAFILE 
EXITCODE="${?}"

# clean up directory
rm *.java *.class  STRUCT*.class Slice.class Cast.class 2> /dev/null


# Lastly, we propagate the exit code
exit $EXITCODE


