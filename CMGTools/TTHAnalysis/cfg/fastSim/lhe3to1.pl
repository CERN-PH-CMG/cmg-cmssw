use strict;
use warnings;

$_ = <>; s{<LesHouchesEvents version="3.0">}{<LesHouchesEvents version="1.0">}; print;

while (<>) { print; m/<MGVersion>/ and last; }
$_ = <>; m/^2\.1\.2/ and s/^/# / and print;

while (<>) { m/<slha>/ and last; print; }
while (<>) { m/<\/slha>/ and last; }

while (<>) {
    m/^<generator/ and next;
    print;
}
