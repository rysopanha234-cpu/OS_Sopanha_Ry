import java.util.concurrent.*;
import java.util.stream.IntStream;

// Task used for Thread, SingleThreadExecutor, CachedThreadPool
class SimpleTask implements Runnable {
    private final int taskId;

    public SimpleTask(int taskId) {
        this.taskId = taskId;
    }

    @Override
    public void run() {
        for (int i = 1; i <= 2; i++) {
            System.out.println(
                "Task " + taskId + " running on " +
                Thread.currentThread().getName() + ": " + i
            );
            try {
                Thread.sleep(1000); // 1 second delay for visualization
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }
}

public class ExecutorDemo {

    public static void main(String[] args) throws InterruptedException {

        // ===============================
        // 1. Thread class
        // ===============================
        System.out.println("=== Thread Class ===");
        Thread t1 = new Thread(new SimpleTask(1));
        t1.start();
        t1.join();

        // ===============================
        // 2. SingleThreadExecutor
        // ===============================
        System.out.println("\n=== SingleThreadExecutor ===");
        ExecutorService singleExecutor = Executors.newSingleThreadExecutor();
        singleExecutor.execute(new SimpleTask(2));
        singleExecutor.execute(new SimpleTask(3));
        singleExecutor.shutdown();
        singleExecutor.awaitTermination(5, TimeUnit.SECONDS);

        // ===============================
        // 3. CachedThreadPool
        // ===============================
        System.out.println("\n=== CachedThreadPool ===");
        ExecutorService cachedExecutor = Executors.newCachedThreadPool();
        for (int i = 4; i <= 8; i++) {
            cachedExecutor.execute(new SimpleTask(i));
        }
        cachedExecutor.shutdown();
        cachedExecutor.awaitTermination(5, TimeUnit.SECONDS);

        // ===============================
        // 4. ForkJoinPool (Parallelism)
        // ===============================
        System.out.println("\n=== ForkJoinPool ===");
        ForkJoinPool forkJoinPool = new ForkJoinPool();

        forkJoinPool.submit(() -> {
            IntStream.range(1, 8).parallel().forEach(n -> {
                System.out.println(
                    "ForkJoin Task " + n + " running on " +
                    Thread.currentThread().getName()
                );
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
        }).join();

        forkJoinPool.shutdown();

        System.out.println("\n=== Program Finished ===");
    }
}
