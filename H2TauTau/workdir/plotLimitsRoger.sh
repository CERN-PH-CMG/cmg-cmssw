rm -f limits_sm.root
for d in 0 1 2 3 4; do
echo $d
rm -f SM$d\_sm.*
plot asymptotic ../sm_htt_layout.py SM$d
done
