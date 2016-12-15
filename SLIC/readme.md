## Simple Linear Iterative Clustering

SLIC is an algorithm used for superpixel generation. When dealing with large images and the feature set is so large that most of the common algorithms are slowed down to the level that they become obsolete. Segmenting the image to superpixels is a great way to reduce the feature set by preserving most of the data. You can read more about [superpixels](http://ttic.uchicago.edu/~xren/research/superpixel/) and [SLIC](http://www.kev-smith.com/papers/SLIC_Superpixels.pdf) in the attached links.

Here is a simple snippet to generate superpixels from an image using `sklearn`. 