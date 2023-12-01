function saveToCppArray(matrix, filename)
% matrix save 2d matrix 矩阵
% file name save文件
    [rows, cols] = size(matrix);

    % Open the file for writing
    fileID = fopen(filename, 'w');
    
    % Check if the file was opened successfully
    if fileID == -1
        error('Failed to open file for writing.');
    end

    % Write the matrix as a C++ unsigned int array in hex format
    fprintf(fileID, 'unsigned int array[%d][%d] = {\n', rows, cols);
    for i = 1:rows
        fprintf(fileID, '    ');
        for j = 1:cols
            fprintf(fileID, '0x%X', matrix(i, j));
            if j ~= cols
                fprintf(fileID, ', ');
            end
        end
        fprintf(fileID, ',\n');
    end
    fprintf(fileID, '};\n');

    % Close the file
    fclose(fileID);
end