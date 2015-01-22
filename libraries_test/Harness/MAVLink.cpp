#include "GCS.h"

void GCS_MAVLINK::handle_guided_request(AP_Mission::Mission_Command& v){ }
void GCS_MAVLINK::handle_change_alt_request(AP_Mission::Mission_Command& v){ }
void GCS_MAVLINK::handleMessage(mavlink_message_t * msg){ }
bool GCS_MAVLINK::try_send_message(enum ap_message id){ return true; }
const AP_Param::GroupInfo GCS_MAVLINK::var_info[1] = {};