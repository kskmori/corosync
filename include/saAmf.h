/*
 * Copyright (c) 2002-2003 MontaVista Software, Inc.
 *
 * All rights reserved.
 *
 * Author: Steven Dake (sdake@mvista.com)
 *
 * This software licensed under BSD license, the text of which follows:
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 *   this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 * - Neither the name of the MontaVista Software, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef AIS_AMF_H_DEFINED
#define AIS_AMF_H_DEFINED

#include "saAis.h"

typedef SaUint64T SaAmfHandleT;

#define SA_AMF_PM_ZERO_EXIT 0x1
#define SA_AMF_PM_NON_ZERO_EXIT 0x2
#define SA_AMF_PM_ABNORMAL_END 0x4

typedef SaUint32T SaAmfPmErrorsT;

typedef enum {
	SA_AMF_PM_PROC = 1,
	SA_AMF_PM_PROC_AND_DESCENDENTS = 2,
	SA_AMF_PM_ALL_PROCESSES =3
} SaAmfPmStopQualifierT;

typedef enum {
	SA_AMF_HEALTHCHECK_AMF_INVOKED = 1,
	SA_AMF_HELATHCHECK_COMPONENT_INVOKED =2
} SaAmfHealthcheckInvocationT;

#define SA_AMF_HEALTHCHECK_KEY_MAX 32
typedef struct {
	SaUint8T key[SA_AMF_HEALTHCHECK_KEY_MAX];
	SaUint16T keyLen;
} SaAmfHealthcheckKeyT;

typedef enum {
	SA_AMF_HA_ACTIVE = 1,
	SA_AMF_HA_STANDBY = 2,
	SA_AMF_HA_QUIESCED = 3,
	SA_AMF_HA_QUIESCING = 4
} SaAmfHAStateT;

#define SA_AMF_CSI_ADD_ONE 0x1
#define SA_AMF_CSI_TARGET_ONE 0x2
#define SA_AMF_CSI_TARGET_ALL 0x4

typedef SaUint32T SaAmfCSIFlagsT;


typedef enum {
	SA_AMF_CSI_NEW_ASSIGN = 1,
	SA_AMF_CSI_QUIESCED = 2,
	SA_AMF_CSI_NOT_QUIESCED = 3,
	SA_AMF_CSI_STILL_ACTIVE = 4,
} SaAmfCSITransitionDescriptorT;

typedef struct {
	SaAmfCSITransitionDescriptorT transitionDescriptor;
	SaNameT activeCompName;
} SaAmfCSIActiveDescriptorT;

typedef struct {
	SaNameT activeCompName;
	SaUint32T standbyRank;
} SaAmfCSIStandbyDescriptorT;

typedef union {
	SaAmfCSIActiveDescriptorT activeDescriptor;
	SaAmfCSIStandbyDescriptorT standbyDescriptor;
} SaAmfCSIStateDescriptorT;

typedef struct {
	SaUint8T *attrName;
	SaUint8T *attrValue;
} SaAmfCSIAttributeT;

typedef struct {
	SaAmfCSIAttributeT *attr;
	SaUint32T number;
} SaAmfCSIAttributeListT;

typedef struct {
	SaAmfCSIFlagsT csiFlags;
	SaNameT csiName;
	SaAmfCSIStateDescriptorT csiStateDescriptor;
	SaAmfCSIAttributeListT csiAttr;
} SaAmfCSIDescriptorT;

typedef struct {
	SaNameT compName;
	SaAmfHAStateT haState;
	SaUint32T rank;
} SaAmfProtectionGroupMemberT;


typedef enum {
	SA_AMF_PROTECTION_GROUP_NO_CHANGE = 1,
	SA_AMF_PROTECTION_GROUP_ADDED = 2,
	SA_AMF_PROTECTION_GROUP_REMOVED = 3,
	SA_AMF_PROTECTION_GROUP_STATE_CHANGE = 4
} SaAmfProtectionGroupChangesT;


typedef struct {
	SaAmfProtectionGroupMemberT member;
	SaAmfProtectionGroupChangesT change;
} SaAmfProtectionGroupNotificationT;

typedef struct {
	SaUint32T numberOfItems;
	SaAmfProtectionGroupNotificationT *notificaton;
} SaAmfProtectionGruopNotificationBufferT;

typedef enum {
	SA_AMF_NO_RECOMMENDATION = 1,
	SA_AMF_COMPONENT_RESTART = 2,
	SA_AMF_COMPONENT_FAILOVER = 3,
	SA_AMF_NODE_SWITCHOVER = 4,
	SA_AMF_NODE_FAILOVER = 5,
	SA_AMF_NODE_FAILFAST = 6,
	SA_AMF_CLUSTER_RESET = 7
} SaAmfRecommendedRecoveryT;

typedef void (*SaAmfHealthcheckCallbackT) (
	SaInvocationT invocation,
	const SaNameT *compName,
	SaAmfHealthcheckKeyT *healthcheckKey);

typedef void (*SaAmfComponentTerminateCallbackT) (
	SaInvocationT invocation,
	const SaNameT *compName);

typedef void (*SaAmfCSISetCallbackT) (
	SaInvocationT invocation,
	const SaNameT *compName,
	SaAmfHAStateT haState,
	SaAmfCSIDescriptorT *csiDescriptor);
			
typedef void (*SaAmfCSIRemoveCallbackT) (
	SaInvocationT invocation,
	const SaNameT *compName,
	const SaNameT *csiName,
	SaAmfCSIFlagsT csiFlags);

typedef void (*SaAmfExternalComponentInstantiateCallbackT) (
	SaInvocationT invocation,
	const SaNameT *proxiedCompName);

typedef void (*SaAmfExternalComponentCleanupCallbackT) (
	SaInvocationT invocation,
	const SaNameT *proxiedCompName);

typedef void (*SaAmfProtectionGroupTrackCallbackT) (
	const SaNameT *csiName,
	SaAmfProtectionGroupNotificationT *notificationBuffer,
	SaUint32T numberOfMembers,
	SaAisErrorT error);

typedef struct {
	SaAmfHealthcheckCallbackT
		saAmfHealthcheckCallback;
	SaAmfComponentTerminateCallbackT
		saAmfComponentTerminateCallback;
	SaAmfCSISetCallbackT
		saAmfCSISetCallback;
	SaAmfCSIRemoveCallbackT
		saAmfCSIRemoveCallback;
	SaAmfProtectionGroupTrackCallbackT
		saAmfProtectionGroupTrackCallback;
	SaAmfExternalComponentInstantiateCallbackT
		saAmfExternalComponentInstantiateCallback;
	SaAmfExternalComponentCleanupCallbackT
		saAmfExternalComponentCleanupCallback;
} SaAmfCallbacksT;

/*
 * Interfaces
 */
#ifdef __cplusplus
extern "C" {
#endif

SaAisErrorT
saAmfInitialize (
	SaAmfHandleT *amfHandle,
	const SaAmfCallbacksT *amfCallbacks,
	SaVersionT *version);

SaAisErrorT
saAmfSelectionObjectGet (
	SaAmfHandleT amfHandle,
	SaSelectionObjectT *selectionObject);

SaAisErrorT
saAmfDispatch (
	SaAmfHandleT amfHandle,
	SaDispatchFlagsT dispatchFlags);

SaAisErrorT
saAmfFinalize (
	SaAmfHandleT amfHandle);

SaAisErrorT
saAmfComponentRegister (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	const SaNameT *proxyCompName);

SaAisErrorT
saAmfComponentUnregister (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	const SaNameT *proxyCompName);

SaAisErrorT
saAmfComponentNameGet (
	SaAmfHandleT amfHandle,
	SaNameT *compName);

SaAisErrorT
saAmfPmStart (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	SaUint64T processId,
	SaInt32T descendentsTreeDepth,
	SaAmfPmErrorsT pmErrors,
	SaAmfRecommendedRecoveryT recommendedRecovery);

SaAisErrorT
saAmfPmStop (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	SaAmfPmStopQualifierT stopQualifier,
	SaInt64T processId,
	SaAmfPmErrorsT pmErrors);

SaAisErrorT
saAmfHealthcheckStart (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	const SaAmfHealthcheckKeyT *healthcheckKey,
	SaAmfHealthcheckInvocationT invocationType,
	SaAmfRecommendedRecoveryT recommendedRecovery);

SaAisErrorT
saAmfHealthcheckConfirm (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	const SaAmfHealthcheckKeyT *healthcheckKey,
	SaAisErrorT healthcheckResult);

SaAisErrorT
saAmfHealthcheckStop (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	const SaAmfHealthcheckKeyT *healthcheckKey);

SaAisErrorT
saAmfHAStateGet (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	const SaNameT *csiName,
	SaAmfHAStateT *haState);


SaAisErrorT
saAmfCSIQuiescingComplete(
	SaAmfHandleT amfHandle,
	SaInvocationT invocation,
	SaAisErrorT error);

SaAisErrorT
saAmfProtectionGroupTrackStart (
	SaAmfHandleT amfHandle,
	const SaNameT *csiName,
	SaUint8T trackFlags,
	const SaAmfProtectionGroupNotificationT *notificationBuffer);

SaAisErrorT
saAmfProtectionGroupTrackStop (
	SaAmfHandleT amfHandle,
	const SaNameT *csiName);

SaAisErrorT
saAmfComponentErrorReport (
	SaAmfHandleT amfHandle,
	const SaNameT *erroneousComponent,
	SaTimeT errorDetectionTime,
	SaAmfRecommendedRecoveryT recommendedRecovery,
	SaNtfIdentifierT ntfIdentifier);

SaAisErrorT
saAmfComponentErrorClear (
	SaAmfHandleT amfHandle,
	const SaNameT *compName,
	SaNtfIdentifierT ntfIdentifier);


SaAisErrorT
saAmfResponse (
	SaAmfHandleT amfHandle,
	SaInvocationT invocation,
	SaAisErrorT error);

#ifdef __cplusplus
}
#endif

#endif /* AIS_AMF_H_DEFINED */
