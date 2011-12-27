#include "flac.h"
#include <string.h>

using namespace FLAC;

FlacDecoder::FlacDecoder(const char *data, size_t length){
    cur = 0;
    dat = data;
    len = length;
    _ok = true;
    set_metadata_respond(FLAC__METADATA_TYPE_VORBIS_COMMENT);
    init();
    process_until_end_of_metadata();
}

void FlacDecoder::error_callback(FLAC__StreamDecoderErrorStatus stat){
    _ok = false;
}

FLAC__StreamDecoderReadStatus FlacDecoder::read_callback(FLAC__byte buf[], size_t *bytes){
    *bytes = (*bytes < len-cur) ? *bytes : len-cur;
    memcpy(buf, dat+cur, *bytes);
    cur += *bytes;
    return *bytes ? FLAC__STREAM_DECODER_READ_STATUS_CONTINUE : FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
}

void FlacDecoder::metadata_callback(const FLAC__StreamMetadata *meta){
    if(meta->type != FLAC__METADATA_TYPE_VORBIS_COMMENT) return;
    Metadata::VorbisComment cmt(meta);
    Metadata::VorbisComment::Entry e;
    for(unsigned i=0; i<cmt.get_num_comments(); i++){
        e = cmt.get_comment(i);
        if(strcmp(e.get_field_name(), "TITLE")==0)
            _title = e.get_field_value();
    }
}
