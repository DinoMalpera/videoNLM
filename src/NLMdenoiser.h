#pragma once

namespace VNLM
{
    
    // forward declarations
    template <typename>
    class Frame;
    template <typename>
    class FrameSequence;
    class NLMparams;
    
    /*  Video Non-Local Means denoiser class.
     *  Performs NLM denoising on the frame of a video,
     *  but instead of using just the one frame to gather statistics
     *  it uses neighbouring frames as well.
     * */
    class NLMdenoiser
    {
        // interface
        
    public:
        template <typename Pixel_Value_policy>
        void
        Denoise(
                const   FrameSequence<Pixel_Value_policy>&  frameSequence,
                        Frame<Pixel_Value_policy>&          result,
                const NLMparams& params );
                
                
        // methods
                
    private:
        template <typename Pixel_Value_policy>
        void
        denoise(
                const   FrameSequence<Pixel_Value_policy>&  frameSequence,
                        Frame<Pixel_Value_policy>&          result,
                const   NLMparams& params );
    };

}
