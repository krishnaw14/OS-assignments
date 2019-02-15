 #!/bin/bash

shell=sshell
shell_source=$1
# bash -x 
./script-a.sh $shell_source
./script-b.sh $shell_source
./script-c.sh $shell_source