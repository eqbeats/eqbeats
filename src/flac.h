#ifndef FLACDEC_H
#define FLACDEC_H

#include <FLAC++/all.h>
#include <string>

class FlacDecoder: public FLAC::Decoder::Stream{

    public:
        FlacDecoder(const char *data, size_t length);
        std::string title() const { return _title; }
        bool ok(){ return _ok; }

    protected:
        FLAC__StreamDecoderWriteStatus write_callback(const FLAC__Frame*, const FLAC__int32* const[]) { return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT; }
        void error_callback(FLAC__StreamDecoderErrorStatus);
        FLAC__StreamDecoderReadStatus read_callback(FLAC__byte buf[], size_t *bytes);
        void metadata_callback(const FLAC__StreamMetadata *meta);

    private:
        std::string _title;
        const char *dat;
        size_t cur;
        size_t len;
        bool _ok;

};

#endif // FLACDEC_H
