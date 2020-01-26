# VideoNLM

Implementation of Non-Local Means noise reduction algorithm for video. Where the basic NLM algorithm collects statistics from single image (single frame, the one being denoised), video NLM takes advantage of the fact that successive frames in video are highly correlated.<br/>
Implemented as in [paper by Buades](https://www.iro.umontreal.ca/~mignotte/IFT6150/Articles/Buades-NonLocal.pdf).

## Dependencies

Compiler supporting C++17<br/>
Boost

## Status

Currently VNLM is in it's first iteration - basic functionallity, nonoptimized (except multithreading).<br/>
Next goal would be to play around with multi-resolution approach - should be way way faster.

## Example

Example use of VNLM for single image using TIFFIO library.

```c++
using namespace VNLM;

std::vector<Color_Space_RGB> frame_data_holder( number_of_pixels );
std::vector<Color_Space_RGB> output_frame( number_of_pixels );

std::transform(
	raster,
	raster+number_of_pixels,
	frame_data_holder.begin(),
	[](const unsigned int& c) -> Color_Space_RGB
	{
		return Color_Space_RGB(
			TIFFGetR(c)/255.0f,
			TIFFGetG(c)/255.0f,
			TIFFGetB(c)/255.0f
		);
	}
	);

Frame<Color_Space_RGB> fr( &frame_data_holder[0], frameSize, 0);
Frame<Color_Space_RGB> outputPic( &output_frame[0], frameSize, 0 );

FrameSequence<Color_Space_RGB> frameSequence;
frameSequence.push_back(fr);

NLMdenoiser nlm;
nlm.Denoise( frameSequence, outputPic );
```


