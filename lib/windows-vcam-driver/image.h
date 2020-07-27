/**************************************************************************
    Constants
**************************************************************************/

// Pixel color for placement onto the synthesis buffer.
typedef enum {
    BLACK = 0,
    WHITE,
    YELLOW,
    CYAN,
    GREEN,
    MAGENTA,
    RED,
    BLUE,

    MAX_COLOR,
    TRANSPARENT,
} COLOR;

// Only useful for text overlay.  This can be substituted for LocX or LocY
// in order to center the text screen on the synthesis buffer.
#define POSITION_CENTER ((ULONG)-1)

/*************************************************
    CImageSynthesizer
    This class synthesizes images in various formats for output from the
    capture filter.  It is capable of performing various text overlays onto
    the image surface.
*************************************************/

class CImageSynthesizer {
protected:
    // The width and height the synthesizer is set to.
    ULONG m_Width;
    ULONG m_Height;

    // The synthesis buffer. All scan conversion happens in the synthesis buffer. This must be set with SetBuffer()
    // before any scan conversion routines are called.
    PUCHAR m_SynthesisBuffer;

    // The default cursor. This is a pointer into the synthesis buffer where a non specific PutPixel will be placed.
    PUCHAR m_Cursor{};
public:
    // Place a pixel at the specified image cursor and move right by one pixel. No bounds checking... wrap around occurs
    virtual void PutPixel(PUCHAR *ImageLocation, COLOR Color) = 0;

    // Place a pixel at the default image cursor and move right by one pixel. No bounds checking... wrap around occurs.
    // If the derived class doesn't provide an implementation, provide one.
    virtual void PutPixel(COLOR Color) {
        PutPixel(&m_Cursor, Color);
    }

    virtual long GetBytesPerPixel() = 0;

    // Get the location into the image buffer for a specific X/Y location. This also sets the synthesizer's default
    // cursor to the position LocX, LocY.
    virtual PUCHAR GetImageLocation(ULONG LocX, ULONG LocY) = 0;

    // Set the image size of the synthesis buffer.
    void SetImageSize(ULONG Width, ULONG Height) {
        m_Width = Width;
        m_Height = Height;
    }

    // Set the buffer the synthesizer generates images to.
    void SetBuffer(PUCHAR SynthesisBuffer) {
        m_SynthesisBuffer = SynthesisBuffer;
    }

    // Synthesize EIA-189-A standard color bars.
    void SynthesizeBars();

    // Overlay a text string onto the image.
    void OverlayText(
            _In_ ULONG LocX,
            _In_ ULONG LocY,
            _In_ ULONG Scaling,
            _In_ LPSTR Text,
            _In_ COLOR BgColor,
            _In_ COLOR FgColor
    );

    // DEFAULT CONSTRUCTOR
    CImageSynthesizer() : m_Width(0), m_Height(0), m_SynthesisBuffer(nullptr) {}

    // CONSTRUCTOR:
    CImageSynthesizer(ULONG Width, ULONG Height) : m_Width(Width), m_Height(Height), m_SynthesisBuffer(nullptr) {}

    // DESTRUCTOR:
    virtual ~CImageSynthesizer() = default;
};

/*************************************************
    CRGB24Synthesizer
    Image synthesizer for RGB24 format.
*************************************************/

class CRGB24Synthesizer : public CImageSynthesizer {
private:
    const static UCHAR Colors[MAX_COLOR][3];
    BOOLEAN m_FlipVertical;
public:
    // Place a pixel at a specific cursor location. *ImageLocation must reside within the synthesis buffer.
    void PutPixel(PUCHAR *ImageLocation, COLOR Color) override {
        if (Color != TRANSPARENT) {
            *(*ImageLocation)++ = Colors[(ULONG) Color][0];
            *(*ImageLocation)++ = Colors[(ULONG) Color][1];
            *(*ImageLocation)++ = Colors[(ULONG) Color][2];
        } else {
            *ImageLocation += 3;
        }
    }

    // Place a pixel at the default cursor location.  The cursor location must be set via GetImageLocation(x, y).
    void PutPixel(COLOR Color) override {
        if (Color != TRANSPARENT) {
            *m_Cursor++ = Colors[(ULONG) Color][0];
            *m_Cursor++ = Colors[(ULONG) Color][1];
            *m_Cursor++ = Colors[(ULONG) Color][2];
        } else {
            m_Cursor += 3;
        }
    }

    long GetBytesPerPixel() override {
        return 3;
    }

    PUCHAR GetImageLocation(ULONG LocX, ULONG LocY) override {
        if (m_FlipVertical) {
            return (m_Cursor = (m_SynthesisBuffer + 3 * (LocX + (m_Height - 1 - LocY) * m_Width)));
        } else {
            return (m_Cursor = (m_SynthesisBuffer + 3 * (LocX + LocY * m_Width)));
        }
    }

    // DEFAULT CONSTRUCTOR:
    explicit CRGB24Synthesizer(BOOLEAN FlipVertical) : m_FlipVertical(FlipVertical) {}

    // CONSTRUCTOR:
    CRGB24Synthesizer(BOOLEAN FlipVertical, ULONG Width, ULONG Height) : CImageSynthesizer(Width, Height),
                                                                         m_FlipVertical(FlipVertical) {}

    // DESTRUCTOR:
    ~CRGB24Synthesizer() override = default;
};

/*************************************************
    CYUVSynthesizer
    Image synthesizer for YUV format.
*************************************************/

class CYUVSynthesizer : public CImageSynthesizer {
private:
    const static UCHAR Colors[MAX_COLOR][3];

    BOOLEAN m_Parity{};
public:
    // Place a pixel at a specific cursor location. *ImageLocation must reside within the synthesis buffer.
    void PutPixel(PUCHAR *ImageLocation, COLOR Color) override {
        BOOLEAN Parity = (((*ImageLocation - m_SynthesisBuffer) & 0x2) != 0);
#if DBG
        // Check that the current pixel points to a valid start pixel
        // in the UYVY buffer.
        BOOLEAN Odd = (((*ImageLocation - m_SynthesisBuffer) & 0x1) != 0);
        NT_ASSERT ((m_Parity && Odd) || (!m_Parity && !Odd));
#endif // DBG
        if (Color != TRANSPARENT) {
            if (Parity) {
                *(*ImageLocation)++ = Colors[(ULONG) Color][2];
            } else {
                *(*ImageLocation)++ = Colors[(ULONG) Color][1];
                *(*ImageLocation)++ = Colors[(ULONG) Color][0];
                *(*ImageLocation)++ = Colors[(ULONG) Color][1];
            }
        } else {
            *ImageLocation += (Parity ? 1 : 3);
        }
    }

    // Place a pixel at the default cursor location. The cursor location must be set via GetImageLocation(x, y).
    void PutPixel(COLOR Color) override {
        if (Color != TRANSPARENT) {
            if (m_Parity) {
                *m_Cursor++ = Colors[(ULONG) Color][2];
            } else {
                *m_Cursor++ = Colors[(ULONG) Color][1];
                *m_Cursor++ = Colors[(ULONG) Color][0];
                *m_Cursor++ = Colors[(ULONG) Color][1];
            }
        } else {
            m_Cursor += (m_Parity ? 1 : 3);
        }
        m_Parity = !m_Parity;
    }

    long GetBytesPerPixel() override {
        return 2;
    }

    PUCHAR GetImageLocation(ULONG LocX, ULONG LocY) override {
        m_Cursor = m_SynthesisBuffer + ((LocX + LocY * m_Width) << 1);
        if (m_Parity = ((LocX & 1) != 0))
            m_Cursor++;
        return m_Cursor;
    }

    // DEFAULT CONSTRUCTOR:
    CYUVSynthesizer() = default;

    // CONSTRUCTOR:
    CYUVSynthesizer(ULONG Width, ULONG Height) : CImageSynthesizer(Width, Height) {}

    // DESTRUCTOR:
    ~CYUVSynthesizer() override = default;
};

