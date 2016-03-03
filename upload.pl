opendir(D, "/c/KinectProjects/Kinect1RecorderPlayer") || die "Can't open directory: $!\n";
while (my $f = readdir(D)) {
    print "\$f = $f\n";
}
closedir(D);