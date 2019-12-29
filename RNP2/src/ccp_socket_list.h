#ifndef msgf_h
#define msgf_h

#define Update_Flag 0
#define MSG_Flag 1


int set_msg_flag(int socket);
int rm_msg_flag(int socket);
int set_up_flag(int socket);
int rm_up_flag(int socket);
int rm_up_flag_indexed(int index);
int rm_msg_flag_indexed(int index);
int retrieve_index_through_socket(int socket);
int remove_contact_in_socket_array_with_index(int index);
int init_socket_list();
int destroy_socket_mutex();
#endif