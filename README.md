# filename-randomizer
A pretty simple cli program that randomize the name of a file,
or if given a directory, randomize the name of all files inside it.


Should work for any normal OS, but only has been tested _(and developed)_ for Linux.
## Usage
```bash
filename-randomizer <options> path
```
The avaible options are the next:
```
[-v --version] Print the version of the program.
[-h --help] Show the help.
[-p --print-changes] Print a list of all the changes made.
[-n --nonstop] If an error is encountered during the name change, it is ignored and the process continues.
```

## Building
This program has been developed with `c++23`, so a compatible compiler is required.

There are not external dependencies besides the standard library.

```bash
cmake -DCMAKE_BUILD_TYPE=Release -B build/
cd build/
cmake --build .
```

## Implementation details.
- No AI or LLM was used in the creation of the program.
- I use modern c++ guidelines.
- The random names are generated using SHA256, using the original filename + a random number.
- The probability of collision is near to zero, but I don't believe in luck, so I implemented a collision detection mechanism, generating a new hash.
- Should work in any platform thanks to `std::filesystem`.
- _mew mew purr_.

##  License
This program is under the [MIT](./LICENSE) License.