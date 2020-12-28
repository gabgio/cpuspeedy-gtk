/*
 * gtk-cpuspeedy
 * http://cpuspeedy.sourceforge.net
 *
 * Copyright (C) Gabriele Giorgetti <gabriele_giorgetti@tin.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <stdio.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
gint cpuspeedy_speed (gchar *value)
{
  char command[1024];
    
  if (CPUSPEEDY_HAVE_PATH)
      strncpy(command,CPUSPEEDY_PATH,sizeof(command));
  else
      strncpy(command,"cpuspeedy", sizeof(command));
  
  strcat (command, " ");
  strncat(command, value,sizeof(command));    
  system (command);
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
gint cpuspeedy_status (gpointer data)
{
  GtkWidget *status_label_widget;
  FILE *pipe;
  char line[4096];
  char status_label[1024];
  char window_title[1024];
  char *freq, *percent, *temp;

  if (CPUSPEEDY_HAVE_PATH)
    {
        char execbuff[1024];
        strncpy(execbuff,CPUSPEEDY_PATH,sizeof(execbuff));
        strcat (execbuff, " --frontend");
        pipe=popen (execbuff, "r");
    }
  else
      pipe=popen ("cpuspeedy --frontend", "r");
  
  if (pipe == NULL)
    return TRUE;

  fgets (line, 4096, pipe);
  pclose (pipe);

  freq = strtok (line, "|");
  percent = strtok (NULL, "|");
  temp = strtok (NULL, "|");

  /* gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button), TRUE); */
  status_label_widget=lookup_widget (data, "label_status");
  
  sprintf (status_label, "<span size=\"medium\" weight=\"bold\">%s (%s) %s</span>", freq, percent, temp);
  
  gtk_label_set_text (status_label_widget, status_label);
  gtk_label_set_use_markup (GTK_LABEL (status_label_widget), TRUE);

  /* set window title to cpu status
   sprintf (window_title, "gtk-cpuspeedy        ---- %s (%s) %s", freq, percent, temp);
   gtk_window_set_title (lookup_widget (data,"window_main"), window_title); */
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
gint quit (GtkWidget * widget, gpointer user_data)
{
  gtk_main_quit ();
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void on_radiobutton_min_toggled (GtkToggleButton * togglebutton, gpointer user_data)
{
  cpuspeedy_speed ("minimum");
  cpuspeedy_status (user_data);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void on_radiobutton_low_toggled (GtkToggleButton * togglebutton, gpointer user_data)
{
  cpuspeedy_speed ("low");
  cpuspeedy_status (user_data);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void on_radiobutton_med_toggled (GtkToggleButton * togglebutton, gpointer user_data)
{
  cpuspeedy_speed ("medium");
  cpuspeedy_status (user_data);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void on_radiobutton_high_toggled (GtkToggleButton * togglebutton, gpointer user_data)
{
  cpuspeedy_speed ("high");
  cpuspeedy_status (user_data);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void on_radiobutton_max_toggled (GtkToggleButton * togglebutton, gpointer user_data)
{
  cpuspeedy_speed ("maximum");
  cpuspeedy_status (user_data);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void on_button_about_pressed (GtkButton * button, gpointer user_data)
{
  GtkWidget *window_about;
  window_about = create_window_about ();
  gtk_widget_show (window_about);
}
