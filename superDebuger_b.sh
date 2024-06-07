#!/bin/bash
gdb kernel/kernel.bin -ex "target remote :1234" 

#-ex "b waitpid" \
#                      -ex "b test.c:1220" \
#                     \
#                      -ex "display ProcElu->pid" \
#                      -ex "display ProcElu->etat" \
#                      -ex "display tableDesProcs[0]->pid" \
#                      -ex "display tableDesProcs[0]->etat" \
                    #   -ex "display tableDesProcs[1]->pid" \
                    #   -ex "display tableDesProcs[1]->etat" \
                    #   -ex "display tableDesProcs[2]->pid" \
                    #   -ex "display tableDesProcs[2]->etat" \
                    #   -ex "display tableDesProcs[3]->pid" \
                    #   -ex "display tableDesProcs[3]->etat"