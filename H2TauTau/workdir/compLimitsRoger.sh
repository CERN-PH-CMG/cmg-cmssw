for d in 0 1 2 3 4; do
echo SM$d
limit.py --asymptotic --expectedOnly  --userOpt '--minosAlgo stepping ' SM$d/*
done
