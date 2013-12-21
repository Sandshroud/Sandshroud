/**
 @file BinaryInput.h
 
 @maintainer Morgan McGuire, http://graphics.cs.williams.edu
 
 @created 2001-08-09
 @edited  2010-03-19

 Copyright 2000-2010, Morgan McGuire.
 All rights reserved.
 */

#ifndef G3D_BinaryInput_h
#define G3D_BinaryInput_h

#ifdef _MSC_VER
// Disable conditional expression is constant, which occurs incorrectly on inlined functions
#   pragma  warning(push)
#   pragma warning( disable : 4127 )
#endif

#include <assert.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include "platform.h"
#include "Array.h"
#include "Color4.h"
#include "Color3.h"
#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "g3dmath.h"
#include "debug.h"
#include "System.h"


namespace G3D {

#if defined(G3D_WIN32) || defined(G3D_LINUX)
    // Allow writing of integers to non-word aligned locations.
    // This is legal on x86, but not on other platforms.
    #define G3D_ALLOW_UNALIGNED_WRITES
#endif

/**
 Sequential or random access byte-order independent binary file access.
 Files compressed with zlib and beginning with an unsigned 32-bit int
 size are transparently decompressed when the compressed = true flag is
 specified to the constructor.

 For every readX method there are also versions that operate on a whole
 Array, std::vector, or C-array.  e.g. readFloat32(Array<float32>& array, n)
 These methods resize the array or std::vector to the appropriate size
 before reading.  For a C-array, they require the pointer to reference
 a memory block at least large enough to hold <I>n</I> elements.

 Most classes define serialize/deserialize methods that use BinaryInput,
 BinaryOutput, TextInput, and TextOutput.  There are text serializer 
 functions for primitive types (e.g. int, std::string, float, double) but not 
 binary serializers-- you <B>must</b> call the BinaryInput::readInt32 or
 other appropriate function.  This is because it would be very hard to 
 debug the error sequence: <CODE>serialize(1.0, bo); ... float f; deserialize(f, bi);</CODE>
 in which a double is serialized and then deserialized as a float. 
 */
class BinaryInput {
private:

    // The initial buffer will be no larger than this, but 
    // may grow if a large memory read occurs.  50 MB
    enum {INITIAL_BUFFER_LENGTH = 50000000};

    /**
     is the file big or little endian
     */
    G3DEndian       m_fileEndian;
    std::string     m_filename;

    bool            m_swapBytes;

    /** Next position to read from in bitString during readBits. */
    int             m_bitPos;

    /** Bits currently being read by readBits.  
        Contains at most 8 (low) bits.  Note that
        beginBits/readBits actually consumes one extra byte, which
        will be restored by writeBits.*/
    g3d_uint32          m_bitString;

    /** 1 when between beginBits and endBits, 0 otherwise. */
    int             m_beginEndBits;

    /** When operating on huge files, we cannot load the whole file into memory.
        This is the file position to which buffer[0] corresponds.
        */
    g3d_int64           m_alreadyRead;

    /**
     Length of the entire file, in bytes.  
     For the length of the buffer, see bufferLength
     */
    g3d_int64           m_length;

    /** Length of the array referenced by buffer. May go past the end of the file!*/
    g3d_int64           m_bufferLength;
    g3d_uint8*          m_buffer;

    /**
     Next byte in file, relative to buffer.
     */
    g3d_int64           m_pos;

    /**
     When true, the buffer is freed in the destructor.
     */
    bool            m_freeBuffer;

    /** Ensures that we are able to read at least minLength from startPosition (relative
        to start of file). */
    void loadIntoMemory(g3d_int64 startPosition, g3d_int64 minLength = 0);

    /** Verifies that at least this number of bytes can be read.*/
    inline void prepareToRead(g3d_int64 nbytes) {
        debugAssertM(m_length > 0, m_filename + " not found or corrupt.");
        debugAssertM(m_pos + nbytes + m_alreadyRead <= m_length, "Read past end of file.");

        if (m_pos + nbytes > m_bufferLength) {
            loadIntoMemory(m_pos + m_alreadyRead, nbytes);    
        }
    }

    // Not implemented on purpose, don't use
    BinaryInput(const BinaryInput&);
    BinaryInput& operator=(const BinaryInput&);
    bool operator==(const BinaryInput&);

    /** Buffer is compressed; replace it with a decompressed version */
    void decompress();
public:

    /** false, constant to use with the copyMemory option */
    static const bool       NO_COPY;

    /**
       If the file cannot be opened, a zero length buffer is presented.
       Automatically opens files that are inside zipfiles.

       @param compressed Set to true if and only if the file was
       compressed using BinaryOutput's zlib compression.  This has
       nothing to do with whether the input is in a zipfile.
    */
    BinaryInput(
        const std::string&  filename,
        G3DEndian           fileEndian,
        bool                compressed = false);

    /**
     Creates input stream from an in memory source.
     Unless you specify copyMemory = false, the data is copied
     from the pointer, so you may deallocate it as soon as the
     object is constructed.  It is an error to specify copyMemory = false
     and compressed = true.

     To decompress part of a file, you can follow the following paradigm:

     <PRE>
        BinaryInput master(...);

        // read from master to point where compressed data exists.

        BinaryInput subset(master.getCArray() + master.getPosition(), 
                           master.length() - master.getPosition(),
                           master.endian(), true, true);

        // Now read from subset (it is ok for master to go out of scope)
     </PRE>
     */
    BinaryInput(
        const g3d_uint8*        data,
        g3d_int64               dataLen,
        G3DEndian           dataEndian,
        bool                compressed = false,
        bool                copyMemory = true);

    virtual ~BinaryInput();

    /** Change the endian-ness of the file.  This only changes the
        interpretation of the file for future read calls; the
        underlying data is unmodified.*/
    void setEndian(G3DEndian endian);

    G3DEndian endian() const {
        return m_fileEndian;
    }

    std::string getFilename() const {
        return m_filename;
    }

    /**
     Returns a pointer to the internal memory buffer.
     May throw an exception for huge files.
     */
    const g3d_uint8* getCArray() const {
        if (m_alreadyRead > 0) {
            throw "Cannot getCArray for a huge file";
        }
        return m_buffer;
    }

    /**
     Performs bounds checks in debug mode.  [] are relative to
     the start of the file, not the current position.
     Seeks to the new position before reading (and leaves 
     that as the current position)
     */
    inline g3d_uint8 operator[](g3d_int64 n) {
        setPosition(n);
        return readUInt8();
    }

    /**
     Returns the length of the file in bytes.
     */
    inline g3d_int64 getLength() const {
        return m_length;
    }

    inline g3d_int64 size() const {
        return getLength();
    }

    /**
     Returns the current byte position in the file,
     where 0 is the beginning and getLength() - 1 is the end.
     */
    inline g3d_int64 getPosition() const {
        return m_pos + m_alreadyRead;
    }

    /**
     Sets the position.  Cannot set past length.
     May throw a char* when seeking backwards more than 10 MB on a huge file.
     */
    inline void setPosition(g3d_int64 p) {
        debugAssertM(p <= m_length, "Read past end of file");
        m_pos = p - m_alreadyRead;
        if ((m_pos < 0) || (m_pos > m_bufferLength)) {
            loadIntoMemory(m_pos + m_alreadyRead);
        }
    }

    /**
     Goes back to the beginning of the file.
     */
    inline void reset() {
        setPosition(0);
    }

    inline g3d_int8 readInt8() {
        prepareToRead(1);
        return m_buffer[m_pos++];
    }

    inline bool readBool8() {
        return (readInt8() != 0);
    }

    inline g3d_uint8 readUInt8() {
        prepareToRead(1);
        return ((g3d_uint8*)m_buffer)[m_pos++];
    }

    g3d_uint16 inline readUInt16() {
        prepareToRead(2);

        m_pos += 2;
        if (m_swapBytes) {
            g3d_uint8 out[2];
            out[0] = m_buffer[m_pos - 1];
            out[1] = m_buffer[m_pos - 2];
            return *(g3d_uint16*)out;
        } else {
            #ifdef G3D_ALLOW_UNALIGNED_WRITES
                return *(g3d_uint16*)(&m_buffer[m_pos - 2]);
            #else
                g3d_uint8 out[2];
                out[0] = m_buffer[m_pos - 2];
                out[1] = m_buffer[m_pos - 1];
                return *(g3d_uint16*)out;
            #endif
        }

    }

    inline g3d_int16 readInt16() {
        g3d_uint16 a = readUInt16();
        return *(g3d_int16*)&a;
    }

    inline g3d_uint32 readUInt32() {
        prepareToRead(4);

        m_pos += 4;
        if (m_swapBytes) {
            g3d_uint8 out[4];
            out[0] = m_buffer[m_pos - 1];
            out[1] = m_buffer[m_pos - 2];
            out[2] = m_buffer[m_pos - 3];
            out[3] = m_buffer[m_pos - 4];
            return *(g3d_uint32*)out;
        } else {
            #ifdef G3D_ALLOW_UNALIGNED_WRITES
                return *(g3d_uint32*)(&m_buffer[m_pos - 4]);
            #else
                g3d_uint8 out[4];
                out[0] = m_buffer[m_pos - 4];
                out[1] = m_buffer[m_pos - 3];
                out[2] = m_buffer[m_pos - 2];
                out[3] = m_buffer[m_pos - 1];
                return *(g3d_uint32*)out;
            #endif
        }
    }


    inline g3d_int32 readInt32() {
        g3d_uint32 a = readUInt32();
        return *(g3d_int32*)&a;
    }

    g3d_uint64 readUInt64();

    inline g3d_int64 readInt64() {
        g3d_uint64 a = readUInt64();
        return *(g3d_int64*)&a;
    }

    inline float32 readFloat32() {
        union {
            g3d_uint32 a;
            float32 b;
        };
        a = readUInt32();
        return b;
    }    

    inline float64 readFloat64() {
        union {
            g3d_uint64 a;
            float64 b;
        };
        a = readUInt64();
        return b;
    }

    void readBytes(void* bytes, g3d_int64 n);

    /**
     Reads an n character string.  The string is not
     required to end in NULL in the file but will
     always be a proper std::string when returned.
     */
    std::string readString(g3d_int64 n);

    /**
     Reads until NULL or the end of the file is encountered.
     */
    std::string readString();

    /** Reads until \r, \r\n, \n\r, \n or the end of the file is encountered. Consumes the newline.*/
    std::string readStringNewline();

    /**
     Reads until NULL or the end of the file is encountered.
     If the string has odd length (including NULL), reads 
     another byte.
     */
    std::string readStringEven();


    std::string readString32();

    Vector4 readVector4();
    Vector3 readVector3();
    Vector2 readVector2();

    Color4 readColor4();
    Color3 readColor3();

    /**
     Skips ahead n bytes.
     */
    inline void skip(g3d_int64 n) {
        setPosition(m_pos + m_alreadyRead + n);
    }

    /**
      Returns true if the position is not at the end of the file
    */
    inline bool hasMore() const {
    return m_pos + m_alreadyRead < m_length;
    }

    /** Prepares for bit reading via readBits.  Only readBits can be
        called between beginBits and endBits without corrupting the
        data stream. */
    void beginBits();

    /** Can only be called between beginBits and endBits */
    g3d_uint32 readBits(int numBits);

    /** Ends bit-reading. */
    void endBits();

#   define DECLARE_READER(ucase, lcase)\
    void read##ucase(lcase* out, g3d_int64 n);\
    void read##ucase(std::vector<lcase>& out, g3d_int64 n);\
    void read##ucase(Array<lcase>& out, g3d_int64 n);

    DECLARE_READER(Bool8,   bool)
    DECLARE_READER(UInt8,   g3d_uint8)
    DECLARE_READER(Int8,    g3d_int8)
    DECLARE_READER(UInt16,  g3d_uint16)
    DECLARE_READER(Int16,   g3d_int16)
    DECLARE_READER(UInt32,  g3d_uint32)
    DECLARE_READER(Int32,   g3d_int32)
    DECLARE_READER(UInt64,  g3d_uint64)
    DECLARE_READER(Int64,   g3d_int64)
    DECLARE_READER(Float32, float32)
    DECLARE_READER(Float64, float64)    
#   undef DECLARE_READER
};


}

#ifdef _MSC_VER
#   pragma  warning(pop)
#endif

#endif
