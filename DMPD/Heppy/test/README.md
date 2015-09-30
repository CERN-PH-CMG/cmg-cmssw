#To lanch jobs using LSF

heppy_batch.py -o Batch treeProducer.py -b 'bsub  < ./batchScript.sh' 

if error is returned, try:

heppy_batch.py -o Batch treeProducer.py -b 'bsub -q local-cms-short  < ./batchScript.sh'
