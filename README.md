# image-preprocessing
This is a design of a preprocessing framework for cleaning MR images. 
The framework will include the following filters: denoising, inhomogeneity (bias) correction and intensity standardization in this order.
Preprocessing filters types are Correcter, Anisotropic and HistogramMatching. 
You are free to use available ITK filters for this purpose. Make sure that your denoising filter will preserve edges while removing nois
