#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linux programming.
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <poll.h>
#include <wait.h>

// Homework related.
#include "logging.h"
#include "message.h"

#define PIPE(fd) socketpair(AF_UNIX, SOCK_STREAM, PF_UNIX, (fd))

static const int server_socket = 0;
static const int client_socket = 1;


void read_and_set_auction_information();
void allocations();
void deallocations();
void create_bidders();
void child_to_client(int file_descriptor);
void server_loop();
void serve_client_message(int index);
void get_message(int index, cm* message);
void send_response(int index, sm* response);
void close_server();
int bid_result(int bid);


int starting_bid, minimum_increment, number_0f_bidders;

int minimum_delay = 0;
int current_bid = 0;
int index_current_bid = -1;

int finished_bidder_count = 0;

struct pollfd* bidder_socket_fds;
pid_t* bidder_pids;
int* bidder_exit_status;


int main(int argc, char* argv[])
{
    read_and_set_auction_information();

    allocations();

    create_bidders();

    server_loop();
    
    close_server();

    deallocations();

    close(0);
    close(1);
    close(2);

    exit(0);
}


void read_and_set_auction_information()
{
    scanf("%d %d %d", &starting_bid, &minimum_increment, &number_0f_bidders);
    current_bid = starting_bid;
}

void allocations()
{
    bidder_socket_fds = calloc(number_0f_bidders, sizeof(struct pollfd));
    bidder_pids = calloc(number_0f_bidders, sizeof(pid_t));
    bidder_exit_status = calloc(number_0f_bidders, sizeof(int));
}

void deallocations()
{
    free(bidder_socket_fds);
    free(bidder_pids);
    free(bidder_exit_status);
}

void create_bidders()
{
    for(int i = 0; i < number_0f_bidders; i++)
    {
        pid_t pid;
        int fd[2];
        PIPE(fd);


        char **child_argv;
        char * path;
        int arg_count;

        scanf("%ms%d", &path, &arg_count);
        child_argv = calloc(arg_count + 2, sizeof(char*));

        child_argv[0] = path;
        for(int i = 1; i <= arg_count; i++)
        {
            char *arg;
            scanf("%ms", &child_argv[i]);
        }
        child_argv[arg_count + 1] = NULL;
        


        if( !(pid = fork()) )
        {
            close(fd[server_socket]);

            // Read bidder info and exec into it.
            dup2(fd[client_socket], 0);
            dup2(fd[client_socket], 1);
            close(fd[client_socket]);

            execv(child_argv[0], child_argv);
        }
        else
        {
            for(int i = 0; i <= arg_count; i++)
                free(child_argv[i]);
            
            free(child_argv);
        }

        close(fd[client_socket]);

        bidder_socket_fds[i].fd = fd[server_socket];
        bidder_socket_fds[i].events = POLLIN;

        bidder_pids[i] = pid;
    }
}

void child_to_client(int file_descriptor)
{
    
}

void server_loop()
{
    while(finished_bidder_count < number_0f_bidders)
    {
        int poll_return =
            poll(bidder_socket_fds, number_0f_bidders, minimum_delay);

        if(poll_return <= 0)
            continue; 
        
        for(int i = 0; i < number_0f_bidders; i++)
        {
            if(bidder_socket_fds[i].revents & POLLIN)
            {
                serve_client_message(i);
            }
        }
    }
}

void serve_client_message(int index)
{
    cm message;  memset(&message , 0, sizeof(cm));
    sm response; memset(&response, 0, sizeof(sm));

    get_message(index, &message);

    switch (message.message_id)
    {
        case CLIENT_CONNECT:
            response.message_id = SERVER_CONNECTION_ESTABLISHED;
            
            if(!minimum_delay || message.params.delay < minimum_delay)
                minimum_delay = message.params.delay;
            
            response.params.start_info.client_id = bidder_pids[index];
            response.params.start_info.starting_bid = starting_bid;
            response.params.start_info.current_bid = current_bid;
            response.params.start_info.minimum_increment = minimum_increment;

            send_response(index, &response);

            break;

        case CLIENT_BID:
            response.message_id = SERVER_BID_RESULT;
            
            response.params.result_info.result = bid_result(message.params.bid);
            if(response.params.result_info.result == BID_ACCEPTED)
            {
                current_bid = message.params.bid;
                index_current_bid = index;
            }

            response.params.result_info.current_bid = current_bid;

            send_response(index, &response);

            break;

        case CLIENT_FINISHED:
            bidder_socket_fds[index].events ^= POLLIN;
            finished_bidder_count++;
            
            bidder_exit_status[index] = message.params.status;

            break;

        default:
            break;
    }
}

void get_message(int index, cm* message)
{
    recv(bidder_socket_fds[index].fd, message, sizeof(cm), 0);

    ii input;
    input.type = message->message_id;
    input.pid = bidder_pids[index];
    input.info = message->params;

    print_input(&input, input.pid);
}

void send_response(int index, sm* response)
{
    oi output;
    output.type = response->message_id;
    output.pid = bidder_pids[index];
    output.info = response->params;

    print_output(&output, output.pid);

    send(bidder_socket_fds[index].fd, response, sizeof(sm), 0);
}

void close_server()
{
    print_server_finished(bidder_pids[index_current_bid], current_bid);

    // inform and finish all bidders
    sm finished_information;
    memset(&finished_information, 0, sizeof(sm));
    
    finished_information.message_id = SERVER_AUCTION_FINISHED;
    finished_information.params.winner_info.winner_id = bidder_pids[index_current_bid];
    finished_information.params.winner_info.winning_bid = current_bid;
    
    // inform all bidders
    for(int i = 0; i < number_0f_bidders; i++)
        send_response(i, &finished_information);

    // finish all bidders
    for(int i = 0; i < number_0f_bidders; i++)
    {
        int stat;

        waitpid(bidder_pids[i], &stat, 0);
        print_client_finished(bidder_pids[i], stat, !(bidder_exit_status[i] ^ stat));

        close(bidder_socket_fds[i].fd);
    }
}

int bid_result(int bid)
{
    int result;
    if(bid < current_bid)
    {
        if(index_current_bid == -1)
        {
            result = BID_LOWER_THAN_STARTING_BID;
        }
        else
        {
            result = BID_LOWER_THAN_CURRENT;
        }
    }
    else if(bid - current_bid < minimum_increment)
    {

        result = BID_INCREMENT_LOWER_THAN_MINIMUM;
    }
    else
    {
        result = BID_ACCEPTED;
    }

    return result;
}