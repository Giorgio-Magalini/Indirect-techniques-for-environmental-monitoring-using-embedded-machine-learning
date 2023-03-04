import os
import requests

in_directory = "" # SET your input directory 

entries = os.listdir(in_directory) # gets every file in directory

for entry in entries: # loop over files in directory

    if not entry.startswith('.') and os.path.isfile(os.path.join(in_directory, entry)): # avoid non wanted files in directory

        path_to_single_file = in_directory + entry # gets path for a single file in directory

        # get labels from filenames
        entry_name = entry.split(".") # separate name from file extension
        label = entry_name[0].split("; ")[1] # pm2.5, pm10
        label0 = label.split(", ")[0] # pm2.5
        label1 = label.split(", ")[1] # pm10
         

        headers = {
            'x-api-key': '', # SET your Edge Impulse project API Key
            'x-label': label1 # insert label
        }

        files = [('data', open(path_to_single_file, 'rb'))] # read files

        res = requests.post('https://ingestion.edgeimpulse.com/api/training/files', headers = headers, files = files) # upload

        #print status
        if (res.status_code == 200):
            print('Uploaded file(s) to Edge Impulse\n', res.status_code, res.content)
        else:
            print('Failed to upload file(s) to Edge Impulse\n', res.status_code, res.content)