#include <QString>
#include <QVector>
#include <utility>

namespace cleanflow {

enum class JobStatus {
    Queued,
    Assigned,
    Starting,
    Processing,
    Validating,
    Completed,
    Failed,
    Retry
};

struct Job {
    QString id;
    QString inputPath;
    QString outputPath;
    QString action;
    JobStatus status = JobStatus::Queued;
    QString workerId;
    QString error;
    int retryCount = 0;
};

class JobManager
{
public:
    void enqueue(Job job) { m_jobs.append(std::move(job)); }
    const QVector<Job>& jobs() const { return m_jobs; }

private:
    QVector<Job> m_jobs;
};

} // namespace cleanflow
