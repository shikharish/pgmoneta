/*
 * Copyright (C) 2024 The pgmoneta community
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its contributors may
 * be used to endorse or promote products derived from this software without specific
 * prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* pgmoneta */
#include <pgmoneta.h>
#include <hot_standby.h>
#include <logging.h>
#include <utils.h>
#include <workers.h>

/* system */
#include <stdlib.h>

static int hot_standby_setup(int, char*, struct node*, struct node**);
static int hot_standby_execute(int, char*, struct node*, struct node**);
static int hot_standby_teardown(int, char*, struct node*, struct node**);

struct workflow*
pgmoneta_create_hot_standby(void)
{
   struct workflow* wf = NULL;

   wf = (struct workflow*)malloc(sizeof(struct workflow));

   wf->setup = &hot_standby_setup;
   wf->execute = &hot_standby_execute;
   wf->teardown = &hot_standby_teardown;
   wf->next = NULL;

   return wf;
}

static int
hot_standby_setup(int server, char* identifier, struct node* i_nodes, struct node** o_nodes)
{
   return 0;
}

static int
hot_standby_execute(int server, char* identifier, struct node* i_nodes, struct node** o_nodes)
{
   char* root = NULL;
   char* source = NULL;
   char* destination = NULL;
   time_t start_time;
   int total_seconds;
   int hours;
   int minutes;
   int seconds;
   char elapsed[128];
   int number_of_workers = 0;
   struct workers* workers = NULL;
   struct configuration* config;

   config = (struct configuration*)shmem;

   if (strlen(config->servers[server].hot_standby) > 0)
   {
      number_of_workers = pgmoneta_get_number_of_workers(server);
      if (number_of_workers > 0)
      {
         pgmoneta_workers_initialize(number_of_workers, &workers);
      }

      start_time = time(NULL);

      source = pgmoneta_append(source, config->base_dir);
      if (!pgmoneta_ends_with(source, "/"))
      {
         source = pgmoneta_append_char(source, '/');
      }

      source = pgmoneta_append(source, config->servers[server].name);
      if (!pgmoneta_ends_with(source, "/"))
      {
         source = pgmoneta_append_char(source, '/');
      }
      
      source = pgmoneta_append(source, "backup/");

      source = pgmoneta_append(source, identifier);
      source = pgmoneta_append_char(source, '/');
      
      source = pgmoneta_append(source, "data/");

      root = pgmoneta_append(root, config->servers[server].hot_standby);
      if (!pgmoneta_ends_with(root, "/"))
      {
         root = pgmoneta_append_char(root, '/');
      }

      destination = pgmoneta_append(destination, root);
      destination = pgmoneta_append(destination, config->servers[server].name);
      if (!pgmoneta_ends_with(destination, "/"))
      {
         destination = pgmoneta_append_char(destination, '/');
      }

      if (pgmoneta_exists(destination))
      {
         pgmoneta_delete_directory(destination);
      }

      pgmoneta_mkdir(root);
      pgmoneta_mkdir(destination);

      pgmoneta_copy_directory(source, destination, NULL, workers);

      pgmoneta_log_trace("hot_standby source:      %s", source);
      pgmoneta_log_trace("hot_standby destination: %s", destination);

      if (number_of_workers > 0)
      {
         pgmoneta_workers_wait(workers);
         pgmoneta_workers_destroy(workers);
      }

      total_seconds = (int)difftime(time(NULL), start_time);
      hours = total_seconds / 3600;
      minutes = (total_seconds % 3600) / 60;
      seconds = total_seconds % 60;

      memset(&elapsed[0], 0, sizeof(elapsed));
      sprintf(&elapsed[0], "%02i:%02i:%02i", hours, minutes, seconds);

      pgmoneta_log_debug("Hot standby: %s/%s (Elapsed: %s)", config->servers[server].name, identifier, &elapsed[0]);
   }

   free(root);
   free(source);
   free(destination);

   return 0;
}

static int
hot_standby_teardown(int server, char* identifier, struct node* i_nodes, struct node** o_nodes)
{
   return 0;
}
