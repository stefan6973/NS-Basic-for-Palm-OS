
@predefTraps =
    (
        sysLibTrapOpen,
        sysLibTrapClose,
        sysLibTrapSleep,
        sysLibTrapWake,
    );

$customTrapNum = 4;
$customTrapName = sysLibTrapCustom;

%sizeof = 
    (
        UInt16 => 2,
        UInt32 => 4,
        Int16 => 2,
        Int32 => 4,
        double => 8,
        Err => 2
    );

print "\nSnip.pl\n";

# Read the libdef file

$line = 0;

open LIBDEF, "libdef" or die "Cannot open libdef";
open DISP1, ">dispTraps.h" or die "Cannot open dispTraps.h";
open DISP2, ">dispHandlers.h" or die "Cannot open dispHandlers.h";
open FUNC, ">funcHandlers.h" or die "Cannot open funcHandlers.h";

@infLines = ();

while (<LIBDEF>)
{
    chomp;
    next if /^\#/;
    next if /^\s*$/;
    if (/\[/)
    {
      push @infLines, $_;
      next;
    }
	   if (/\=/)
	   {
      push @infLines, $_;

						if ($` eq PrcName)
      {
        $prcName = $';
        ($prefix = $prcName) =~ s/\..*//;
      }
      next;
	   }

    if (/\(/)
    {
        # Break it up into typename and args

        $typeName = $`;
        $args = $';

        if ($typeName =~ s/func *//)
        {
          $role = func;
        }
        else
        {
          $role = proc;
        }

        if ($args =~ s/\/\/ *(.*)$//)
        {
            $comment = $1;
        }
        else
        {
            $comment = "";
        }

        $args =~ s/\)//;
        @args = split(/,/, $args);

        if ($typeName =~ s/(\w*)$//)
        {
            $type = $`;
            $name = $1;
        }
        else
        {
            print "Bad type and name: $typeName\n";
            next;
        }
        $type =~ s/\s//g;

        # Emit the global methods stuff
        $gm = "";

        ($gmName = $name) =~ s/$prefix//;
        $gm .= $gmName . '=' . ($line - 3) . ", $role, ";
        $nBasicArgs = (($role eq proc) ? -1 : -2);
								

        print "Type: $type\n";
        print "Name: $name\n";
        print "Args:\n";
        foreach $arg (@args)
        {
            if ($arg =~ s/(\w*)$//)
            {
                $argType = $`;
                $argName = $1;
                ++$nBasicArgs;
            }
            else
            {
                print "Bad arg type and name: $argTypeName\n";
                next;
            }
            $argType =~ s/\s//g;
            print "    Type: $argType\n";
            print "    Name: $argName\n";
        }
        $gm .= $nBasicArgs . ', "' . $comment . '"';
        push (@infLines, $gm) if $line > 3;

        # Emit the dispatch trap
        print DISP1 "	DC.W		libDispatchEntry($line)					// $name\n";

        # Emit the dispatch handler
        print DISP2 "\@Goto$name:
	JMP 		$name\n";

        # Emit the function handler
        if ($line < $customTrapNum)
        {
            $trap = $predefTraps[$line];
        }
        else
        {
            $ln2 = $line - $customTrapNum;
            $trap = "$customTrapName + $ln2";
        }
        print FUNC "extern $type $name($args)
				SAMPLE_LIB_TRAP($trap);\n";
    }
    else
    {
        print "Bad line $_\n";
    }
    ++$line;
}

close FUNC;
close DISP2;
close DISP1;
close LIBDEF;

# Emit the offset size
open OFFSET, ">dispOffset.h" or die "Cannot open dispOffset.h";

$lnp1 = $line + 1;
print OFFSET "#define	kOffset		(2*$lnp1)						// NOTE: This is empirical!!!!!!
";

close OFFSET;

open INF, ">$prefix.inf" or die "Cannot open $prefix.inf";
print INF join("\n", @infLines) . "\n";
close INF;