## dicepwgen - A diceware password generator

This is the README file for the password generator dicepwgen.

## Documentation

You can read the documentation without installing the
software:

    perldoc dicepwgen.pod

If it is already installed, you can read the manual page:

    man dicepwgen

## Installation

This software doesn't have any external dependencies, but
you need either BSD make or GNU make installed to build it.

First you need to check out the source code. Skip this, if
you have already done so:

    git clone git@github.com:TLINDEN/diceware.git

Next, change into the newly created directory 'diceware' and
compile the source code:

    cd diceware
    make

Although dicepwgen has an option to specify a dictionary file
on the commandline, there's also a built-in default. This default
can be modified during compilation, e.g.:

    make DICTFILE=/usr/local/share/dict/german.txt

To install, type this command:

    sudo make install

This will install the binary to `$PREFIX/sbin/dicepwgen` and
the manual page to `$PREFIX/man/man1/dicepwgen.1`. You can
modify `$PREFIX` during installation time like this:

   make install PREFIX=/opt


## Usage

    Usage: dice [-tcfvhd]
    Options: 
    -t --humantoss            Asks interactively for rolled dices
    -c --wordcount <count>    Number of words (default: 4)
    -f --dictfile <dictfile>  Dictionary file to use (default:
                              /usr/share/dict/american-english)
    -l --minlen <count>       Minimum word len (default: 5)
    -m --maxlen <count>       Maximum word len (default: 10)
    -n --dontjump             Use all words in the dict file, e.g.
                              if it is an original diceware list   
    -d --debug                Enable debug output
    -v --version              Print program version
    -h -? --help              Print this help screen

If you want to generate a truly random diceware password, use
the option -t. In that case you have to roll physical dices and
enter the results (dicepwgen will ask you for them).

## Getting help

Although I'm happy to hear from dicepwgen users in private email,
that's the best way for me to forget to do something.

In order to report a bug, unexpected behavior, feature requests
or to submit a patch, please open an issue on github:
https://github.com/TLINDEN/diceware/issues.

## License

This software is licensed under the GNU GENERAL PUBLIC LICENSE version 3.

## Author

T.v.Dein <tom AT vondein DOT org>

## Project homepage

https://github.com/TLINDEN/diceware
