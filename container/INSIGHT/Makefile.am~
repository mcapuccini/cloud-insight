#######################################
# The list of libraries we are building seperated by spaces.
# The 'lib_' indicates that these build products will be installed
# in the $(libdir) directory. For example /usr/lib
ACLOCAL_AMFLAGS         = -I m4
				
bin_PROGRAMS = INSIGHTv3

INSIGHTv3_SOURCES = src/fileIO/BGReader.cpp src/fileIO/DataReader.cpp src/fileIO/FcsProcessor.cpp \
					src/fileIO/InsightOutputWriter.cpp src/fileIO/IOUtils.cpp src/fileIO/PrevPopReader.cpp \
					src/fileIO/ProblemFileReader.cpp \
					src/models/BirthDeathFlModel.cpp src/models/BirthDeathModel.cpp src/models/LacGfp7Model.cpp \
					src/models/Model.cpp src/models/ModelFactory.cpp src/models/ModelFl.cpp\
					src/sampler/DistributionKernel.cpp \
					src/sampler/KernelSamplerFactory.cpp src/sampler/KernelParticleSampler.cpp \
					src/sampler/ScaledUniformKernel.cpp src/sampler/ScaledUniformSamplerFactory.cpp \
					src/sampler/UniformKernel.cpp src/sampler/UniformSampler.cpp src/sampler/UniformSamplerFactory.cpp \
					src/FixedSequenceToleranceProvider.cpp src/InsightAlgorithm.cpp src/InsightIteration.cpp \
					src/INSIGHTv3.cpp src/IterationLogger.cpp src/KolmogorovComputer.cpp \
					src/MathUtils.cpp src/ParticleEvaluatorInsight.cpp src/ParticleSet.cpp \
					src/SSASimulator.cpp src/StateBundle.cpp
					
INSIGHTv3_CPPFLAGS = $(Eigen3_CFLAGS) -I$(top_srcdir)/src/fileIO -I$(top_srcdir)/src/models -I$(top_srcdir)/src/sampler -I$(top_srcdir)/src

if USE_MPI
bin_PROGRAMS += INSIGHTv3_mpi
INSIGHTv3_mpi_SOURCES = src/fileIO/BGReader.cpp src/fileIO/DataReader.cpp src/fileIO/FcsProcessor.cpp \
						src/fileIO/InsightOutputWriter.cpp src/fileIO/IOUtils.cpp src/fileIO/PrevPopReader.cpp \
						src/fileIO/ProblemFileReader.cpp \
						src/models/BirthDeathFlModel.cpp src/models/BirthDeathModel.cpp src/models/LacGfp7Model.cpp \
						src/models/Model.cpp src/models/ModelFactory.cpp src/models/ModelFl.cpp\
						src/sampler/DistributionKernel.cpp \
						src/sampler/KernelSamplerFactory.cpp src/sampler/KernelParticleSampler.cpp \
					src/sampler/ScaledUniformKernel.cpp src/sampler/ScaledUniformSamplerFactory.cpp \
						src/sampler/UniformKernel.cpp src/sampler/UniformSampler.cpp src/sampler/UniformSamplerFactory.cpp \
						src/FixedSequenceToleranceProvider.cpp src/InsightAlgorithm.cpp src/InsightIteration.cpp src/InsightIterationMpi.cpp \
						src/InsightMpiWorker.cpp src/INSIGHTv3_mpi.cpp src/IterationLogger.cpp src/KolmogorovComputer.cpp \
						src/MathUtils.cpp src/ParticleEvaluatorInsight.cpp src/ParticleSet.cpp \
						src/SSASimulator.cpp src/StateBundle.cpp

INSIGHTv3_mpi_CPPFLAGS = $(Eigen3_CFLAGS) -I$(top_srcdir)/src/fileIO -I$(top_srcdir)/src/models -I$(top_srcdir)/src/sampler -I$(top_srcdir)/src

INSIGHTv3_mpi_LINK = mpicxx -o $@ ${LDFLAGS}
endif

