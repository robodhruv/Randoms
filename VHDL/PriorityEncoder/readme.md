## Priority Encoder
__Credits:__ _[Shashwat Shukla](https://github.com/ShashShukla), [Dhruv Ilesh Shah](https://github.com/PrieureDeSion)_

As you must be aware, a priority encoder is one of the simplest ways to encode an 8-bit string to a 3-bit representation, based on the MSB. A simple representation of the working can be seen as follows:

![Priority Encoder Logic](http://www.electronicshub.org/wp-content/uploads/2015/06/Octal-to-Binary-Priority-Encoder.jpg)


The logic is trivial, and the implementation is to be done from an 8-bit string to a 3-bit string (4-bit considering the enable). The implementation can be found in `PriorityEncoder.vhd`. The component has been instantiated in the file `DUT.vhd` and then in the `Testbench.vhd`

Since the number of cases in the `TRACEFILE.txt` would be exponential, I have used a python script to generate it as per requirements. This can be found as `generate_tracefile.py`.
