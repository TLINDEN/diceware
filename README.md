## dicepwgen - A diceware password generator

This is the README file for the password generator dicepwgen.

## Documentation

You can read the documentation without installing the
software:

    perldoc udpxd.pod

If it is already installed, you can read the manual page:

    man udpxd

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

To install, type this command:

    sudo make install

This will install the binary to `$PREFIX/sbin/dicepwgen` and
the manual page to `$PREFIX/man/man1/dicepwgen.1`. You can
modify `$PREFIX` during installation time like this:

   make install PREFIX=/opt


## Usage

```
Usage: dice [-tcfvh]
Options: 
-t               Asks interactively for tossed dices
-c <count>       Number of words (default: 4)
-f <dictfile>    Dictionary file to use (default:
                 /usr/share/dict/american-english)
-v               Print program version
-h               Print this help screen
```

## Getting help

Although I'm happy to hear from udpxd users in private email,
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
