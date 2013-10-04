#!/usr/bin/env perl

use strict;
use Fcntl;

sub isDone;

my $argc = @ARGV;
my $fullCommand = $ARGV[0];
my $command = $fullCommand;
$command =~ s/[[:space:]]*([^[:space:]]*).*/$1/;
my @clusters = @ARGV[1 .. $argc - 1];
my $clusters = join (" ", @clusters);

while (1)
  {
    sleep 60;
    if (isDone ($clusters))
      {
        close (OUTPUT);
        system ("$fullCommand > $command.out.$$ 2>&1");
        last;
      }
  }

sub
isDone
{
  my $clusters = shift;

  my @condorQ = `LD_LIBRARY_PATH=/usr/lib64/condor:\$LD_LIBRARY_PATH condor_q $clusters`;
  my $condorQ = @condorQ;

  return ($condorQ == 4);
}
