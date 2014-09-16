use strict;
use warnings;

$_ = <>; s{<LesHouchesEvents version="3.0">}{<LesHouchesEvents version="1.0">}; print;

while (<>) { print; m/<MGVersion>/ and last; }
$_ = <>; m/^2\.1\.2/ and s/^/# / and print;

while (<>) { m/<slha>/ and last; m/sys_alpsfact/ and next; print; }
while (<>) { m/<\/slha>/ and last; }

while (<>) {
    m/^<generator/ and next;
    m/sys_alpsfact/ and next; # fix bug
    print;
}
