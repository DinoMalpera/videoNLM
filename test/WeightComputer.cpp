#include <gtest/gtest.h>
#include <src/WeightComputer.h>
#include <src/FrameSequence.h>
#include <src/NLMparams.h>
#include <src/policies/DistributionFunction_policy.h>
#include <src/policies/DistanceFunction_policy.h>
#include <src/policies/PatchDistance_policy.h>
#include <src/Pixel_Value.h>

using namespace VNLM;

static
auto
test_params(
	const FrameSize		frameSize,
	const Pixel_Coord	pt,
	const Pixel_Coord	pt_s,
	const unsigned int	patch_size,
	const unsigned int	patch_radius,
	const float			filtering_parameter,
	const float			standard_deviation_of_noise )
{
	const Pixel_Range_Iterator it{ pt, patch_size };
	const FrameSequence_Pixel_Range::iterator it_s{
		Pixel_Range_Iterator{ pt_s, patch_size },
		Pixel_Range_Iterator{ {patch_size,patch_size}, patch_size },
		{0,0},
		0 };
	const NLMparams params{ patch_radius, 0, standard_deviation_of_noise, filtering_parameter };
		
	return std::tuple( frameSize, pt, pt_s, it, it_s, params );
}

TEST( WeightComputerTest, BasicsSingleFrame )
{
	const auto [ frameSize, pt, pt_s, it, it_s, params ] = test_params( { 4, 4 }, { 1, 1 }, { 2, 2 }, 3, 1, 1.0f, 0.0f );
	 
	std::vector<float> myFrame{	0.1375f,	0.3375f,	0.01625f,	0.2025f,
								0.07125f,	0.515f,		0.99625f,	0.05125f,
								0.28875f,	0.175f,		0.375f,		0.235f,
								0.01375f,	0.10375f,	0.1925f,	0.2775f };
	
	Frame fr( &myFrame[0], frameSize, 0 );
	
	FrameSequence<Color_Space_Grayscale> frameSequence;
	frameSequence.push_back( fr );
	frameSequence.set_center_frame_index( 0 );
	const double res = WeightComputer::weight<	Color_Space_Grayscale,
												Gauss,
												Element2Element<	Color_Space_Grayscale,
																	NormL2<Color_Space_Grayscale>>>
		(	it,
			it_s,
			frameSequence,
			params );
	
	ASSERT_NEAR( res, 0.87211f, 0.00001f );
}

TEST( WeightComputerTest, BasicsSingleFrameSelf )
{
	const auto [ frameSize, pt, pt_s, it, it_s, params ] = test_params( { 4, 4 }, { 1, 1 }, { 1, 1 }, 3, 1, 1.0f, 0.0f );

	std::vector<float> myFrame{	0.1375f,	0.3375f,	0.01625f,	0.2025f,
								0.07125f,	0.515f,		0.99625f,	0.05125f,
								0.28875f,	0.175f,		0.375f,		0.235f,
								0.01375f,	0.10375f,	0.1925f,	0.2775f };

	Frame fr( &myFrame[0], frameSize, 0 );

	FrameSequence<Color_Space_Grayscale> frameSequence;
	frameSequence.push_back( fr );
	frameSequence.set_center_frame_index( 0 );
	const double res = WeightComputer::weight<	Color_Space_Grayscale,
												Gauss,
												Element2Element<	Color_Space_Grayscale,
																	NormL2<Color_Space_Grayscale>>>
		(	it,
			it_s,
			frameSequence,
			params );

	ASSERT_NEAR( res, 1.0f, 0.00001f );
}

