/*
 * Copyright (c) 2019, Intel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.

 *
 */

//!
//! \file:   OmafAdaptationSet.h
//! \brief:
//! \detail:
//! Created on May 24, 2019, 10:19 AM
//!

#ifndef OMAFADAPTATIONSET_H
#define OMAFADAPTATIONSET_H

#include "general.h"
#include "OmafSegment.h"
#include "OmafDashParser/BaseUrlElement.h"
#include "OmafDashParser/AdaptationSetElement.h"
#include "OmafDashParser/DescriptorElement.h"
#include "OmafDashParser/RepresentationElement.h"

VCD_OMAF_BEGIN

using namespace VCD::VRVideo;
using namespace VCD::OMAF;

const static uint8_t recordSize = 3;

//!
//! \class:   OmafAdaptationSet
//! \brief:
//!

class OmafAdaptationSet {
public:
    //!
    //! \brief  construct
    //!
    OmafAdaptationSet();

    //!
    //! \brief  construct from AdaptationSetElement
    //!
    OmafAdaptationSet( AdaptationSetElement* pAdaptationSet, ProjectionFormat pf );

    //!
    //! \brief  de-construct
    //!
    virtual ~OmafAdaptationSet();

public:

    int LoadLocalInitSegment();

    int LoadLocalSegment();

    int LoadAssignedInitSegment(std::string assignedSegment);

    OmafSegment* LoadAssignedSegment(std::string assignedSegment);

    //!
    //! \brief  Get InitializeSegment for reading.
    //!
    int DownloadInitializeSegment();

    //!
    //! \brief  Download Segment for reading.
    //!
    int DownloadSegment( );

    //!
    //! \brief  Select representation from
    //!
    int  SelectRepresentation( );

    //!
    //! \brief  update start number for download based on stream start time
    //! \param  nAvailableStartTime : the start time for live stream in mpd
    //!
    int  UpdateStartNumberByTime(uint64_t nAvailableStartTime);

    //!
    //! \brief  Initialize the AdaptationSet
    //!
    int Initialize(AdaptationSetElement* pAdaptationSet);

    //!
    //! \brief  Get head segment in segment list which have been downloaded
    //!
    OmafSegment* GetNextSegment();

    OmafSegment* GetLocalNextSegment();

    //!
    //! \brief  Seek to special segment and is is valid in static mode
    //!
    int SeekTo( int seg_num );

    //!
    //! \brief  The following methods are basic Get/Set for fields
    //!
    void                      SetTrackNumber(int nTrackNumber)             { mTrackNumber = nTrackNumber; };
    void                      SetMediaType(MediaType type)                 { mType = type;                };
    void                      SetProjectionFormat(ProjectionFormat format) { mPF = format;                };
    int                       GetTrackNumber()                             { return mTrackNumber;         };
    void                      SetBaseURL(std::vector<BaseUrlElement *> url )     { mBaseURL = url;              };
    OmafSrd*                  GetSRD()                                     { return mSRD;                 };
    int                       GetID()                                      { return mID;                  };
    std::vector<int>          GetDependIDs()                               { return mDependIDs;           };
    std::string               GetMimeType()                                { return mMimeType;            };
    std::vector<std::string>  GetCodec()                                   { return mCodec;               };
    OmafSegment*              GetInitSegment()                             { return mInitSegment;         };
    bool                      IsMain()                                     { return m_bMain;              };
    RwpkType                  GetRegionWisePacking()                       { return mRwpkType;            };
    SphereQuality*            GetQualityRanking()                          { return mSrqr;                };
    PreselValue*              GetPreselection()                            { return mPreselID;            };
    TwoDQualityRanking*       GetTwoDQualityRanking()                      { return mTwoDQuality;         };
    ContentCoverage*          GetContentCoverage()                         { return mCC;                  };
    FramePackingType          GetFramePackingType()                        { return mFpt;                 };
    ProjectionFormat          GetProjectionFormat()                        { return mPF;                  };
    VideoInfo                 GetVideoInfo()                               { return mVideoInfo;           };
    AudioInfo                 GetAudioInfo()                               { return mAudioInfo;           };
    MediaType                 GetMediaType()                               { return mType;                };
    uint64_t                  GetSegmentDuration()                         { return mSegmentDuration;     };
    uint32_t                  GetStartNumber()                             { return mStartNumber;         };
    std::string               GetRepresentationId()                        { return mRepresentation->GetId(); };
    uint32_t                  GetRepresentationQualityRanking()            { return stoi(mRepresentation->GetQualityRanking());};
    int                       Enable( bool bEnable )
    {
        mEnableRecord.push_back(bEnable);
        while(mEnableRecord.size() > recordSize)
            mEnableRecord.pop_front();

        // Set mReEnable true if mEnableRecord is {false, false, true}, it's because
        // everytime Adaption Set is set as true, it must be set as false before.
        // check OmafMediaStream::UpdateEnabledExtractors for more details
        if(bEnable && !mEnable && mEnableRecord.size() == recordSize && mEnableRecord.front() == false)
            mReEnable = true;

        mEnable = bEnable;
        return 0;
    };
    bool                      IsEnabled()                                  { return mEnable;              };

    virtual OmafAdaptationSet* GetClassType(){
        return this;
    };

    TileDef*                   GetTileInfo()                               { return mTileInfo;            };

private:

    //!
    //! \brief  Decide whether this adaption set is main Adaption Set;
    //!         the Main AS will no depend any other AS and has initial seg
    //!
    void JudgeMainAdaptationSet();

    void ClearSegList();

friend class RepresentationSelector;

protected:
    AdaptationSetElement                 *mAdaptationSet;    //<! the libdash Adaptation set
    RepresentationElement                *mRepresentation;   //<! the selected representation
    std::list<OmafSegment*>               mSegments;         //<! active segments list
    OmafSegment*                          mInitSegment;      //<! Initialize Segmentation

    OmafSrd                              *mSRD;              //<! SRD information
    TileDef                              *mTileInfo;          //<! TileDef information
    PreselValue                          *mPreselID;         //<! dependency ID
    TwoDQualityRanking                   *mTwoDQuality;      //<! TwoDQUality ranking
    SphereQuality                        *mSrqr;             //<! srqe information
    ContentCoverage                      *mCC;               //<! Content Coverage
    RwpkType                              mRwpkType;         //<! mRwpkType
    FramePackingType                      mFpt;              //<! FramePackingType
    ProjectionFormat                      mPF;               //<! Projection Fromat
    int                                   mID;               //<! the ID of this adaption Set. ?trackID
    std::vector<int>                      mDependIDs;         //<! the ID this adaption Set depends on
    uint64_t                              mSegmentDuration;  //<! Segment duration as advertised in the MPD
    int                                   mStartNumber;      //<! the first number of segment after getting
                                                             //<! mpd which is used to get first segment for downloading
    int                                   mActiveSegNum;     //<! the segment are being processed
    int                                   mSegNum;           //<! the segment count
    bool                                  m_bMain;           //<! whether this AdaptationSet is Main or not. each stream
                                                             //<! has one main AdaptationSet

    std::vector<BaseUrlElement *>         mBaseURL;          //<! the base url
    int                                   mTrackNumber;      //<! the track number in mp4 for this AdaptationSet
    pthread_mutex_t                       mMutex;
    std::list<SampleData*>                mSampleList;       //<! a queue to storce all readed Sample

    VideoInfo                             mVideoInfo;        //<! Video relative information
    AudioInfo                             mAudioInfo;        //<! Audio relative information
    std::string                           mMimeType;         //<! MineType like "video/mp4 profile='hevi'"
    std::vector<std::string>              mCodec;            //<! codec string
    MediaType                             mType;             //<! media type
    bool                                  mEnable;           //<! is Adaptation Set enabled
    bool                                  mReEnable;         //<! flag for Adaption Set is re-enabled
    std::list<bool>                       mEnableRecord;     //<! record the last 3 enable changes
};

VCD_OMAF_END;

#endif /* OMAFAdaptationSet_H */

