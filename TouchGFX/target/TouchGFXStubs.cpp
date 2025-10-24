// TouchGFX stubs for missing library functions
// These functions are not used in this project but are referenced by generated code

#include <touchgfx/hal/Types.hpp>
#include <cstddef>

namespace touchgfx
{
    // Forward declarations
    class GlyphNode;

    // Stub implementations for CompressedFontCache
    namespace CompressedFontCache
    {
        bool hasCachedGlyph(const GlyphNode* /*glyph*/)
        {
            return false; // No cache available
        }

        void cacheGlyph(const GlyphNode* /*glyph*/, const uint8_t* /*data*/)
        {
            // No-op: caching not implemented
        }
    }

    // Stub implementations for CompressedUnmappedFontCache
    namespace CompressedUnmappedFontCache
    {
        bool hasCachedGlyphNode(const GlyphNode* /*glyph*/)
        {
            return false; // No cache available
        }

        bool hasCachedGlyphData(const GlyphNode* /*glyph*/, const uint8_t*& /*data*/)
        {
            return false; // No cache available
        }
    }

    // Stub implementation for VectorFontRendererImpl
    // Define the class and its method
    class VectorFontRendererImpl
    {
    public:
        void getVectorFontBuffers(float*& vectorFontData, int& vectorFontDataSize, 
                                 unsigned char*& glyphBuffer, int& glyphBufferSize);
    };

    void VectorFontRendererImpl::getVectorFontBuffers(float*& vectorFontData, int& vectorFontDataSize, 
                                                       unsigned char*& glyphBuffer, int& glyphBufferSize)
    {
        // No-op: vector fonts not used
        vectorFontData = NULL;
        vectorFontDataSize = 0;
        glyphBuffer = NULL;
        glyphBufferSize = 0;
    }
}
