   #!/bin/bash
   # Directory where the file will be copied
   git log --format="%H" "filename" > commit_hashes.txt
   export_dir="common_folder"
   # Create the export directory if it doesn't exist
   mkdir -p $export_dir
   # File you want to export
   file_to_export="filename"
   # File containing the list of commit hashes
   commits_file="commit_hashes.txt"

   # Read each commit hash from the file
   while read -r commit_hash
   do
     # Checkout the file at the specific commit
     git checkout $commit_hash -- $file_to_export

     # Copy the file to the export directory with the commit hash as part of the filename
     cp $file_to_export $export_dir/${commit_hash}_${file_to_export}
   done < "$commits_file"

   echo "Files exported successfully to $export_dir"

