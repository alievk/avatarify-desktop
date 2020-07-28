

/*************************************************
    CImageSynthesizer
    This class synthesizes images in various formats for output from the capture filter.
    It is capable of performing various text overlays onto the image surface.
*************************************************/
class CImageSynthesizer {
protected:
    ULONG m_bytesPerPixel;
    ULONG m_Width;
    ULONG m_Height;
    PUCHAR m_inBuffer;
    PUCHAR m_outBuffer;

public:
    void SetImageSize(ULONG Width, ULONG Height) {
        m_Width = Width;
        m_Height = Height;
    }

    void setInBuffer(PUCHAR buffer) {
        m_inBuffer = buffer;
    }

    void setOutBuffer(PUCHAR buffer) {
        m_outBuffer = buffer;
    }

    ULONG GetBytesPerPixel() const {
        return m_bytesPerPixel;
    }

    void UpdateFrame() const;

    explicit CImageSynthesizer(ULONG bytesPerPixel) : m_bytesPerPixel(bytesPerPixel), m_Width(0), m_Height(0),
                                                      m_inBuffer(nullptr), m_outBuffer(nullptr) {}

    virtual ~CImageSynthesizer() = default;
};

/*************************************************
    CRGB24Synthesizer
    Image synthesizer for RGB24 format.
*************************************************/
class CRGB24Synthesizer : public CImageSynthesizer {
private:
    BOOLEAN m_FlipVertical;
public:
    explicit CRGB24Synthesizer(BOOLEAN FlipVertical) : CImageSynthesizer(3), m_FlipVertical(FlipVertical) {}

    ~CRGB24Synthesizer() override = default;
};

/*************************************************
    CYUVSynthesizer
    Image synthesizer for YUV format.
*************************************************/
class CYUVSynthesizer : public CImageSynthesizer {
public:
    CYUVSynthesizer() : CImageSynthesizer(2) {}

    ~CYUVSynthesizer() override = default;
};

