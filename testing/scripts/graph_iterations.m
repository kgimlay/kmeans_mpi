% Author: Kevin Imlay
% Graph the iterations until convergence


max_iterations = 10;
data_size = 10000;
data_dimensionality = 2;
num_clusters = 5;
dataset_name = 'M2.csv';
clust_output = 'clusters.csv';
point_ass_output = 'point_assignment.csv';

base_dir = '../School/graduate/CS685\ -\ Graduate\ Research/kmeans_mpi';
base_dir_sans_esc = erase(base_dir, '\');
path_export = 'export PATH="/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"';
kmeans_call_args = strcat(strcat('mpiexec -np 2 kmeans MPI_LLOYD ./datasets/%s', sprintf(' %d %d %d ./output/', data_size, data_dimensionality, num_clusters)), ' -i %d');

dataset_dir = '/datasets/%s';
output_dir = '/output/%s';
img_out_dir = '/testing/output/%s';
dataset_file = strcat(base_dir_sans_esc, sprintf(dataset_dir, dataset_name));
clust_file = strcat(base_dir_sans_esc, sprintf(output_dir, clust_output));
ass_file = strcat(base_dir_sans_esc, sprintf(output_dir, point_ass_output));
img_file = strcat(base_dir_sans_esc, sprintf(img_out_dir, "%d.jpg"));


% loop
% up to the number of iterations to run
% you should run independently first to find the number of iterations to
% convergence
for iteration = 0:max_iterations
    % run kmeans with iteration counter as max iterations
    system_call = sprintf('%s; cd %s; %s', path_export, base_dir, sprintf(kmeans_call_args, dataset_name, iteration));
    system(system_call);
    
    % load dataset and output files
    assignments = [readmatrix(dataset_file) readmatrix(ass_file)];
    clusters = readmatrix(clust_file);
    
    % parse data into clusters
    data_size = size(assignments);
    dim_size = data_size(end) - 1;
    num_data = data_size(1);
    num_centr = size(clusters);
    num_centr = num_centr(1) - 1;
    
    sortrows(assignments, dim_size + 1);
    for centroidIdx = 0:num_centr
        grouped_data = assignments(assignments(:,dim_size + 1) == centroidIdx,:);
        
        % seperate dimensions
        group_dim_data = grouped_data(1:end,1:1);
        centr_dim_data = clusters(1:end,1:1);
        for dimIdx = 2:dim_size
            group_dim_data(:,:,dimIdx) = grouped_data(1:end, dimIdx:dimIdx);
            centr_dim_data(:,:,dimIdx) = clusters(1:end, dimIdx:dimIdx);
        end
        
        % plot points and centroids
        %tempPlot = figure('visible', 'off');
        if dim_size == 2
            scatter(group_dim_data(:,:,1), group_dim_data(:,:,2), 75, 'filled')
            hold on
            scatter(centr_dim_data(:,:,1), centr_dim_data(:,:,2), 125, 'd', 'MarkerEdgeColor', 'black', 'MarkerFaceColor', 'black')
        else
            disp("Dimensions other than 2 not supported")
            return
        end
    end
    hold off
        
    % save plot
    title(sprintf("%s (start of iteration %d)", dataset_name, iteration))
    exportgraphics(gcf, sprintf(img_file, iteration));
end