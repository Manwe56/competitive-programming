package competitive.programming.genetic;

/**
 * @author Manwe
 *
 * Interface to generate randomly a new candidate during the genetic algorithm
 * 
 * @param <Genotype>
 *  The class representing one candidate
 */
public interface CandidateGenerator<Genotype> {
    /**
     * @return
     * Return a new instance of Genotype that has been generated randomly
     */
    Genotype generateRandomly();
}
