/*
 * Copyright (c) 2018, Intel Corporation
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
 */


/*
 * File:   OmafDashAccessApi.h
 * Author: Zhang, Andrew
 *
 * Created on May 22, 2019, 1:09 PM
 */

#ifndef OMAFDASHACCESSAPI_H
#define OMAFDASHACCESSAPI_H

#include <stdint.h>
#include "error.h"
#include "data_type.h"
#include "360SCVPAPI.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* Handler;

/*
 * the enum for source type; more type will be added in future
 */
typedef enum{
    DefaultSource = 0,
    MultiResSource,
    Reserved
} SourceType;

/*
 * media_url : the url of mpd file to be opened. only mpd can be supported now
 * source_type : the structure of videos in the mpd to be processed
 * cache_path : the directory to store cached downloaded files; a default path
 *              will be used if it is ""
 */
typedef struct DASHSTREAMINGCLIENT{
    const char*        media_url;
    SourceType         source_type;
    const char*        cache_path;
    bool               enable_extractor;
} DashStreamingClient;

/*
 * description: API to initialize API handle and relative context
 * params: pCtx - [in] the structure for the necessary parameters to handle an dash stream
 * return: the handle created for the API
 */
Handler OmafAccess_Init( DashStreamingClient* pCtx);

/*
 * description: API to open a dash stream
 * params: hdl - [in] handler created with DashStreaming_Init
 *         pCtx - [in] the structure for the necessary parameters to handle an dash stream
 *         enablePredictor - [in] flag for use predictor or not
 *         predictPluginName - [in] name of predict plugin
 *         libPath - [in] plugin library path
 * return: the error return from the API
 */
int OmafAccess_OpenMedia( Handler hdl, DashStreamingClient* pCtx, bool enablePredictor, char *predictPluginName, char *libPath);

/*
 * description: API to seek a stream. only work with static mode. not implement yet.
 * params: hdl - [in] handler created with DashStreaming_Init
 *         time - [in] the position to be seek
 * return: the error return from the API
 */
int OmafAccess_SeekMedia( Handler hdl, uint64_t time );

/*
 * description: API to close a dash stream
 * params: hdl - [in]handler created with DashStreaming_Init
 * return: the error return from the API
 */
int OmafAccess_CloseMedia( Handler hdl );

/*
 * description: API to get information of opened dashed stream
 * params: hdl - [in] handler created with DashStreaming_Init
 *         info - [out] the media info of opened dash media
 * return: the error return from the API
 */
int OmafAccess_GetMediaInfo( Handler hdl, DashMediaInfo* info );

/*
 * description: API to get packets according to stream id in the dash media. As for viewport-based
 * Tile dashing streaming with low Resolution video, the packet is composed of viewport
 * -wise tiles and low-res tiles.
 * params: hdl - [in]handler created with DashStreaming_Init
 *         stream_id - [in] the stream id the packet is gotten from
 *         size - [out] the size of gotten packet;
 *         buf  - [out] the payload of the packet;
 *         pts  - [out] the timestamp of the packet
 *         needParams - [bool] flag to include VPS/SPS/PPS in packet
 *         clearBuf - [bool] flag to clear output packet buffer
 * return: the error return from the API, ERROR_EOS means reach end of
 *         stream for static source
 */
int OmafAccess_GetPacket( Handler hdl, int stream_id, DashPacket* packet, int* size, uint64_t* pts, bool needParams, bool clearBuf );

/*
 * description: API to set InitViewport before downloading segment.
 * params: hdl - [in]handler created with DashStreaming_Init
 *         clientInfo - [in] the headset info which is needed to calculate viewport
 * return: the error return from the API
 */
int OmafAccess_SetupHeadSetInfo( Handler hdl, HeadSetInfo* clientInfo);

/*
 * description: API to update Viewport when input shows that viewport is changed
 * params: hdl - [in]handler created with DashStreaming_Init
 *         pose - [in] changed pose info
 * return: the error return from the API
 */
int OmafAccess_ChangeViewport( Handler hdl, HeadPose* pose);

/*
 * description: API to get statistic data such as bandwith etc.
 * params: hdl - [in] handler created with DashStreaming_Init
 *         info - [out] the information current statistic data
 * return: the error return from the API
 */
int OmafAccess_Statistic( Handler hdl, DashStatisticInfo* info );

/*
 * description: API to Close the Handle and release relative resources after dealing with
 * the media
 * params: hdl - [in] handler created with DashStreaming_Init
 * return: the error return from the API
 */
int OmafAccess_Close( Handler hdl );


#ifdef __cplusplus
}
#endif

#endif /* VRDASHSTREAMINGAPI_H */


