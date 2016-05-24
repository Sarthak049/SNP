#include <gtk/gtk.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
static void create_new_mount( GtkWidget *widget,GtkWidget *entry )
{
	pid_t x;
	int exist = 0;
	char path[100],user[50],filep[100],temp[50];
	const gchar *entry_text;
	FILE *fp;
  	entry_text = gtk_entry_get_text (GTK_ENTRY (entry));
  	printf ("Creating Mount: %s\n", entry_text);
	strcpy(path,"/home/");
	getlogin_r(user,50);
	strcat(path,user);
	strcpy(filep,path);
	strcat(filep,"/cryptodev/modentry.dat");
	fp = fopen(filep,"a+");
	while(fscanf(fp,"%s\n",temp)!=EOF)
	{
		if(strcmp(temp,entry_text)==0)
		{
			exist = 1;
		}
	}
	if(exist!=1)
	{
		fprintf(fp,"%s\n",entry_text);
	}
	fclose(fp);
	x = fork();
	if(x == 0)
	{
		strcat(path,"/cryptodev/temp");
		execlp(path,"temp",entry_text,NULL);
		_exit(0);
	} 
	else
	{
		wait(NULL);
		g_print("Parent\n");
		//Parent Code
	}
}

static void r_button_pressed(GtkWidget *widget,GtkWidget *entry, gpointer data )
{
	pid_t z;
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), TRUE);
	char user[50],path[100];
	char *x = (char *)data;	
	if(strlen(x)!=0)
	{
		strcpy(path,"/home/");
		getlogin_r(user,50);
		strcat(path,user);
		z = fork();
		if(z==0)
		{
			strcat(path,"/cryptodev/application");
			execlp(path,"application",x,NULL);
			_exit(0);
		}
		wait(NULL);
		/*z=fork();
		if(z == 0)
		{
			strcat(path,"/backup");
			execlp(path,"backup",NULL);
			_exit(0);
		}
		g_print("Saved\n");*/
	}
}

static gboolean delete_event (GtkWidget *widget, GdkEvent *event, gpointer data)
{
    g_print ("delete event occurred\n");
    return FALSE;
}

static void destroy( GtkWidget *widget, gpointer data)
{
	g_print("Destroy Callback\n");
    gtk_main_quit ();
}

static void refresh_list( GtkWidget *widget, GtkWidget *vbox1)
{
	int count = 0;
	char path[100],user[50],temp[50];
	FILE *fp;
	GSList *group;
	GtkWidget *button,*label,*scrolled_window,*vbox2;
	GList *children, *iter;

	children = gtk_container_get_children(GTK_CONTAINER(vbox1));
	for(iter = children; iter != NULL; iter = g_list_next(iter))
  		gtk_widget_destroy(GTK_WIDGET(iter->data));
	g_list_free(children);
	
	strcpy(path,"/home/");
	getlogin_r(user,50);
	strcat(path,user);
	strcat(path,"/cryptodev/modentry.dat");
	fp = fopen(path,"a+");

	button = gtk_button_new_with_label ("Refresh");
    	gtk_box_pack_start (GTK_BOX (vbox1), button, FALSE, FALSE, 0);
    	g_signal_connect (button,"clicked",G_CALLBACK (refresh_list), vbox1);
    	gtk_widget_show (button);
	
	scrolled_window = gtk_scrolled_window_new (NULL, NULL);    
	gtk_container_set_border_width (GTK_CONTAINER (scrolled_window), 1);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX(vbox1),scrolled_window,TRUE,TRUE,0);
	gtk_widget_show(scrolled_window);

	vbox2 = gtk_vbox_new (FALSE, 0);
	gtk_scrolled_window_add_with_viewport (GTK_SCROLLED_WINDOW (scrolled_window), vbox2);
    	gtk_widget_show (vbox2);

	while(fscanf(fp,"%s\n",temp)!=EOF)
	{
		if(count == 0)
		{
			button = gtk_radio_button_new_with_label (NULL, temp);
    			gtk_box_pack_start (GTK_BOX (vbox2), button, FALSE, FALSE, 0);
			group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button));
		}
		else if(count == 1)
		{
			button = gtk_radio_button_new_with_label (group, temp);
    			gtk_box_pack_start (GTK_BOX (vbox2), button, FALSE, FALSE, 0);
		}
		else
		{
			button = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (button),temp);
    			gtk_box_pack_start (GTK_BOX (vbox2), button, FALSE, FALSE, 0);
		}
		char* x = strdup(temp);	
		g_signal_connect(button,"button_release_event",G_CALLBACK(r_button_pressed),x);	// NEW
    		gtk_widget_show (button);
		count++;
	}
	fclose(fp);
	if(count == 0)
	{
		label = gtk_label_new ("Click on Refresh Button\n"\
				" to search for Mounts");
    		gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
    		gtk_box_pack_start(GTK_BOX(vbox2),label,TRUE,TRUE,0);
    		gtk_widget_show(label);
	}
}

static void entry_toggle_visibility( GtkWidget *checkbutton, GtkWidget *entry )
{
  gtk_entry_set_visibility (GTK_ENTRY (entry), GTK_TOGGLE_BUTTON (checkbutton)->active);
}

static void entry_toggle_editable( GtkWidget *checkbutton, GtkWidget *entry )
{
  gtk_editable_set_editable (GTK_EDITABLE (entry), GTK_TOGGLE_BUTTON (checkbutton)->active);
}


int main( int argc, char *argv[] )
{
    GtkWidget *window;
    GtkWidget *vbox,*vbox1, *hbox,*hbox1;
    GtkWidget *entry;
    GtkWidget *button;
    GtkWidget *check;
    GtkWidget *frame;
    GtkWidget *label;
    gint tmp_pos;
    gtk_init (&argc, &argv);
	//Window Creation code
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    g_signal_connect (window, "delete-event",G_CALLBACK (delete_event), NULL);
    g_signal_connect (window, "destroy",G_CALLBACK (destroy), NULL);
    gtk_widget_set_size_request (GTK_WIDGET (window), 400,250);
    gtk_window_set_title (GTK_WINDOW (window), "User Application");
    //gtk_window_set_resizable (GTK_WINDOW(window), FALSE);
	//Main Horizontal Box
    hbox1 = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), hbox1);
    gtk_widget_show (hbox1);	
	//Frame 1
    frame = gtk_frame_new ("New Mount");
    gtk_box_pack_start(GTK_BOX(hbox1),frame,TRUE,TRUE,0);
    gtk_widget_show(frame);
	//Vertical Box
    vbox = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (frame), vbox);
    gtk_widget_show (vbox);
	//Text Entry Code
    entry = gtk_entry_new ();
    gtk_entry_set_max_length (GTK_ENTRY (entry), 50);
    gtk_entry_set_text (GTK_ENTRY (entry), "dummydev");
    tmp_pos = GTK_ENTRY (entry)->text_length;
    gtk_editable_select_region (GTK_EDITABLE (entry),0, GTK_ENTRY (entry)->text_length);
    gtk_box_pack_start (GTK_BOX (vbox), entry, TRUE, TRUE, 0);
    gtk_widget_show (entry);
	//Frame 2
    frame = gtk_frame_new ("Available Mounts");
    gtk_box_pack_start(GTK_BOX(hbox1),frame,TRUE,TRUE,0);
    gtk_widget_show(frame);
	//Vertical Box 2
    vbox1 = gtk_vbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (frame), vbox1);
    gtk_widget_show (vbox1);
	//Horizontal Layout
    hbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (vbox), hbox);
    gtk_widget_show (hbox);
	//CheckBox1
    check = gtk_check_button_new_with_label ("Editable");
    gtk_box_pack_start (GTK_BOX (hbox), check, TRUE, TRUE, 0);
    g_signal_connect (check, "toggled",G_CALLBACK (entry_toggle_editable), entry);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gtk_widget_show (check);
	//CheckBox2
    check = gtk_check_button_new_with_label ("Visible");
    gtk_box_pack_start (GTK_BOX (hbox), check, TRUE, TRUE, 0);
    g_signal_connect (check, "toggled",G_CALLBACK (entry_toggle_visibility), entry);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (check), TRUE);
    gtk_widget_show (check);
	//Button action
    button = gtk_button_new_with_label ("Create Mount");
    g_signal_connect (button, "clicked",G_CALLBACK (create_new_mount), entry);
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    gtk_widget_set_can_default (button, TRUE);
    gtk_widget_grab_default (button);
    gtk_widget_show (button);
	//Buttn2
    button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
    g_signal_connect_swapped (button, "clicked",G_CALLBACK (gtk_widget_destroy),window);
    gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);
    gtk_widget_show (button);
	//CheckBoxes
    button = gtk_button_new_with_label ("Refresh");
    gtk_box_pack_start (GTK_BOX (vbox1), button, FALSE, FALSE, 0);
    g_signal_connect (button,"clicked",G_CALLBACK (refresh_list), vbox1);
    gtk_widget_show (button);    
	//New Label
    label = gtk_label_new ("Click on Refresh Button\n"\
				" to search for Mounts");
    gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
    gtk_box_pack_start(GTK_BOX(vbox1),label,TRUE,TRUE,0);
    gtk_widget_show(label);
	//Adding Component to the main window
    //gtk_container_add (GTK_CONTAINER (window), button);
    //gtk_widget_show (button);
    gtk_widget_show (window);
    gtk_main ();
    
    return 0;
}
